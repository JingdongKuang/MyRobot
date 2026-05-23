#include "SimServoAdapter.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <stdexcept>

namespace
{
constexpr double kPi = 3.14159265358979323846;
constexpr double kServoMinRad = -kPi;
constexpr double kServoMaxRad = kPi;
constexpr double kServoMaxCount = 1023.0;
}  // namespace

namespace myrobot_ros2
{
SimServoAdapter::SimServoAdapter(
  rclcpp::Node & node,
  std::shared_ptr<myexternal::ServoInterface> servoInterface,
  std::vector<std::string> jointNames)
: servoInterface_(std::move(servoInterface)),
  jointNames_(std::move(jointNames)),
  logger_(node.get_logger())
{
  if (!servoInterface_) {
    throw std::invalid_argument("servoInterface must not be null");
  }
  if (jointNames_.size() != servoInterface_->JointCount()) {
    throw std::invalid_argument("jointNames size must match ServoInterface::JointCount()");
  }

  jointStatePub_ = node.create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);
  jointCommandSub_ = node.create_subscription<sensor_msgs::msg::JointState>(
    "/joint_command", 10,
    std::bind(&SimServoAdapter::OnJointCommand, this, std::placeholders::_1));
}

void SimServoAdapter::ReadAndPublish(const rclcpp::Time & stamp)
{
  sensor_msgs::msg::JointState msg;
  msg.header.stamp = stamp;
  msg.name = jointNames_;

  const auto positions = servoInterface_->ReadJointPositions();
  msg.position.reserve(positions.size());
  for (const auto p : positions) {
    msg.position.push_back(ServoPositionToRad(p));
  }

  jointStatePub_->publish(msg);
}

std::uint16_t SimServoAdapter::RadToServoPosition(double rad)
{
  const auto clamped = std::clamp(rad, kServoMinRad, kServoMaxRad);
  const auto normalized = (clamped - kServoMinRad) / (kServoMaxRad - kServoMinRad);
  const auto value = normalized * kServoMaxCount;
  return static_cast<std::uint16_t>(std::llround(value));
}

double SimServoAdapter::ServoPositionToRad(std::uint16_t position)
{
  const double normalized = static_cast<double>(position) / kServoMaxCount;
  return kServoMinRad + normalized * (kServoMaxRad - kServoMinRad);
}

void SimServoAdapter::OnJointCommand(const sensor_msgs::msg::JointState::SharedPtr msg)
{
  if (!msg) {
    return;
  }
  if (msg->position.size() != servoInterface_->JointCount()) {
    RCLCPP_WARN(
      logger_,
      "Ignore /joint_command: expected %zu positions, got %zu",
      servoInterface_->JointCount(),
      msg->position.size());
    return;
  }

  std::vector<std::uint16_t> command;
  command.reserve(msg->position.size());
  for (const auto rad : msg->position) {
    command.push_back(RadToServoPosition(rad));
  }
  servoInterface_->SetJointPositions(command);
}
}  // namespace myrobot_ros2
