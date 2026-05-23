#include "Controller.h"
#include "MockForceSensor.h"
#include "MockTransport.h"
#include "PIDController.h"
#include "ServoController.h"
#include "ServoInterface.h"
#include "SimForceSensorAdapter.h"
#include "SimServoAdapter.h"
#include "TrajectoryPlanner.h"

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
class RobotNode : public rclcpp::Node
{
public:
  RobotNode()
  : rclcpp::Node("myrobot_node")
  {
    const auto defaultIds = std::vector<int64_t>{1, 2, 3, 4, 5, 6};
    const auto defaultNames = std::vector<std::string>{
      "joint1", "joint2", "joint3", "joint4", "joint5", "joint6"};

    this->declare_parameter<std::vector<int64_t>>("joint_ids", defaultIds);
    this->declare_parameter<std::vector<std::string>>("joint_names", defaultNames);
    this->declare_parameter<int>("control_period_ms", 20);
    this->declare_parameter<std::string>("force_frame_id", "tool0");

    const auto jointIds64 = this->get_parameter("joint_ids").as_integer_array();
    auto jointNames = this->get_parameter("joint_names").as_string_array();
    const auto controlPeriodMs = this->get_parameter("control_period_ms").as_int();
    const auto forceFrameId = this->get_parameter("force_frame_id").as_string();

    if (jointIds64.empty()) {
      throw std::invalid_argument("joint_ids must not be empty");
    }
    if (jointNames.size() != jointIds64.size()) {
      jointNames.resize(jointIds64.size());
      for (std::size_t i = 0; i < jointNames.size(); ++i) {
        if (jointNames[i].empty()) {
          jointNames[i] = "joint" + std::to_string(i + 1);
        }
      }
    }

    std::vector<std::uint8_t> jointIds;
    jointIds.reserve(jointIds64.size());
    for (const auto id : jointIds64) {
      jointIds.push_back(static_cast<std::uint8_t>(std::clamp<int64_t>(id, 0, 255)));
    }

    auto transport = std::make_shared<myservo::MockTransport>();
    auto servoController = std::make_shared<myservo::ServoController>(transport);
    servoInterface_ = std::make_shared<myexternal::ServoInterface>(servoController, jointIds);
    forceSensor_ = std::make_shared<myexternal::MockForceSensor>();

    planner_ = std::make_unique<myplanning::LinearTrajectoryPlanner>(20);
    controller_ = std::make_unique<mycontrol::PIDController>(0.8, 0.0, 0.0, jointIds.size());
    servoAdapter_ =
      std::make_unique<myrobot_ros2::SimServoAdapter>(*this, servoInterface_, std::move(jointNames));
    forceAdapter_ =
      std::make_unique<myrobot_ros2::SimForceSensorAdapter>(*this, forceSensor_, forceFrameId);

    goalPositionRad_.assign(servoInterface_->JointCount(), 0.0);

    goalSub_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "/joint_goal", 10,
      std::bind(&RobotNode::OnJointGoal, this, std::placeholders::_1));

    forceSensor_->Open();
    forceSensor_->Zero();

    const auto period = std::chrono::milliseconds(std::max(1, controlPeriodMs));
    const auto dt = std::chrono::duration<double>(period).count();

    timer_ = this->create_wall_timer(
      period,
      [this, dt]() { this->OnControlTick(dt); });

    RCLCPP_INFO(this->get_logger(), "myrobot_node started with %zu joints", servoInterface_->JointCount());
  }

  ~RobotNode() override
  {
    if (forceSensor_) {
      forceSensor_->Close();
    }
  }

private:
  void OnJointGoal(const sensor_msgs::msg::JointState::SharedPtr msg)
  {
    if (!msg) {
      return;
    }
    if (msg->position.size() != goalPositionRad_.size()) {
      RCLCPP_WARN(
        this->get_logger(),
        "Ignore /joint_goal: expected %zu positions, got %zu",
        goalPositionRad_.size(),
        msg->position.size());
      return;
    }
    goalPositionRad_ = msg->position;
  }

  void OnControlTick(double dt)
  {
    const auto rawFeedback = servoInterface_->ReadJointPositions();
    std::vector<double> feedbackRad;
    feedbackRad.reserve(rawFeedback.size());
    for (const auto p : rawFeedback) {
      feedbackRad.push_back(myrobot_ros2::SimServoAdapter::ServoPositionToRad(p));
    }

    const auto trajectory = planner_->Plan(feedbackRad, goalPositionRad_);
    const auto & reference = trajectory.points.empty() ? goalPositionRad_ : trajectory.points.back();

    const auto output = controller_->Compute(reference, feedbackRad, dt);

    std::vector<std::uint16_t> command;
    command.reserve(output.size());
    for (std::size_t i = 0; i < output.size(); ++i) {
      const auto cmdRad = feedbackRad[i] + output[i];
      command.push_back(myrobot_ros2::SimServoAdapter::RadToServoPosition(cmdRad));
    }
    servoInterface_->SetJointPositions(command);

    const auto now = this->now();
    servoAdapter_->ReadAndPublish(now);
    forceAdapter_->PublishOnce(now);
  }

  std::shared_ptr<myexternal::ServoInterface> servoInterface_;
  std::shared_ptr<myexternal::MockForceSensor> forceSensor_;

  std::unique_ptr<myplanning::IPlanner> planner_;
  std::unique_ptr<mycontrol::IController> controller_;
  std::unique_ptr<myrobot_ros2::SimServoAdapter> servoAdapter_;
  std::unique_ptr<myrobot_ros2::SimForceSensorAdapter> forceAdapter_;

  std::vector<double> goalPositionRad_;

  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr goalSub_;
  rclcpp::TimerBase::SharedPtr timer_;
};
}  // namespace

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  try {
    rclcpp::spin(std::make_shared<RobotNode>());
  } catch (const std::exception & e) {
    RCLCPP_FATAL(rclcpp::get_logger("myrobot_node"), "Fatal exception: %s", e.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
