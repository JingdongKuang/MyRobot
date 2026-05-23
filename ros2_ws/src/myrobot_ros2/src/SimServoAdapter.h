#pragma once

#include "ServoInterface.h"

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

namespace myrobot_ros2
{
class SimServoAdapter
{
public:
  SimServoAdapter(
    rclcpp::Node & node,
    std::shared_ptr<myexternal::ServoInterface> servoInterface,
    std::vector<std::string> jointNames);

  void ReadAndPublish(const rclcpp::Time & stamp);

  static std::uint16_t RadToServoPosition(double rad);
  static double ServoPositionToRad(std::uint16_t position);

private:
  void OnJointCommand(const sensor_msgs::msg::JointState::SharedPtr msg);

  std::shared_ptr<myexternal::ServoInterface> servoInterface_;
  std::vector<std::string> jointNames_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr jointStatePub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr jointCommandSub_;
  rclcpp::Logger logger_;
};
}  // namespace myrobot_ros2
