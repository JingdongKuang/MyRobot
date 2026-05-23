#include "SimForceSensorAdapter.h"

#include <stdexcept>
#include <utility>

namespace myrobot_ros2
{
SimForceSensorAdapter::SimForceSensorAdapter(
  rclcpp::Node & node,
  std::shared_ptr<myexternal::IForceSensor> forceSensor,
  std::string frameId)
: forceSensor_(std::move(forceSensor)), frameId_(std::move(frameId))
{
  if (!forceSensor_) {
    throw std::invalid_argument("forceSensor must not be null");
  }
  wrenchPub_ = node.create_publisher<geometry_msgs::msg::WrenchStamped>("/force_torque", 10);
}

void SimForceSensorAdapter::PublishOnce(const rclcpp::Time & stamp)
{
  const auto reading = forceSensor_->Read();

  geometry_msgs::msg::WrenchStamped msg;
  msg.header.stamp = stamp;
  msg.header.frame_id = frameId_;
  msg.wrench.force.x = reading.force[0];
  msg.wrench.force.y = reading.force[1];
  msg.wrench.force.z = reading.force[2];
  msg.wrench.torque.x = reading.torque[0];
  msg.wrench.torque.y = reading.torque[1];
  msg.wrench.torque.z = reading.torque[2];

  wrenchPub_->publish(msg);
}
}  // namespace myrobot_ros2
