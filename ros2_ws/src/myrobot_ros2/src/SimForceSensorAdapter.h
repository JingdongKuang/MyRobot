#pragma once

#include "IForceSensor.h"

#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <rclcpp/rclcpp.hpp>

#include <memory>
#include <string>

namespace myrobot_ros2
{
class SimForceSensorAdapter
{
public:
  SimForceSensorAdapter(
    rclcpp::Node & node,
    std::shared_ptr<myexternal::IForceSensor> forceSensor,
    std::string frameId);

  void PublishOnce(const rclcpp::Time & stamp);

private:
  std::shared_ptr<myexternal::IForceSensor> forceSensor_;
  std::string frameId_;
  rclcpp::Publisher<geometry_msgs::msg::WrenchStamped>::SharedPtr wrenchPub_;
};
}  // namespace myrobot_ros2
