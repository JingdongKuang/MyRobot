#pragma once

#include <array>
#include <cstdint>

namespace myexternal
{
// 六维力/力矩读数：Fx, Fy, Fz, Tx, Ty, Tz。
// 力的单位为 N，力矩单位为 N·m。
struct ForceReading
{
    std::array<double, 3> force{ 0.0, 0.0, 0.0 };
    std::array<double, 3> torque{ 0.0, 0.0, 0.0 };

    // 采样时间戳（自设备启动以来的微秒数，或上位机统一时钟）。
    std::uint64_t timestampUs{ 0 };
};
}
