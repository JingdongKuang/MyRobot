#pragma once

#include <string>

namespace myrobot
{
enum class JointType
{
    kRevolute,   // 旋转关节
    kPrismatic,  // 平动关节
    kFixed,      // 固定关节
    kContinuous, // 连续旋转关节（无限制）
};

// 单个关节的机械描述。
struct Joint
{
    std::string name;
    JointType   type{ JointType::kRevolute };

    std::string parentLink;
    std::string childLink;

    // 关节角度/位移限位（弧度或米）。
    double lowerLimit{ 0.0 };
    double upperLimit{ 0.0 };

    // 最大速度与最大力矩/力。
    double velocityLimit{ 0.0 };
    double effortLimit{ 0.0 };
};
}
