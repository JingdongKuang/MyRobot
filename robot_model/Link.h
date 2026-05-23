#pragma once

#include <array>
#include <string>

namespace myrobot
{
// 单个连杆的机械描述。
struct Link
{
    std::string name;

    // 质量（kg）。
    double mass{ 0.0 };

    // 质心相对连杆坐标系的位置 (x, y, z)，单位 m。
    std::array<double, 3> centerOfMass{ 0.0, 0.0, 0.0 };

    // 惯性张量：Ixx, Iyy, Izz, Ixy, Ixz, Iyz。
    std::array<double, 6> inertia{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
};
}
