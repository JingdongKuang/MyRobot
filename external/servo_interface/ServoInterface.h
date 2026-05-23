#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace myservo { class ServoController; }

namespace myexternal
{
// 多关节伺服适配层：把底层单舵机协议聚合成“机器人关节组”级别的接口。
// 上层（控制环、规划器）只需面对一组关节，无需感知协议细节。
class ServoInterface
{
public:
    // ids: 关节对应的伺服 ID 列表。顺序即关节顺序。
    ServoInterface(std::shared_ptr<myservo::ServoController> controller,
                   std::vector<std::uint8_t> ids);

    // 检测所有伺服是否在线。
    bool PingAll() const;

    // 下发一组关节目标位置（与构造时的 ids 一一对应）。
    void SetJointPositions(const std::vector<std::uint16_t>& positions) const;

    // 回读所有关节当前位置。
    std::vector<std::uint16_t> ReadJointPositions() const;

    std::size_t JointCount() const noexcept { return m_ids.size(); }

private:
    std::shared_ptr<myservo::ServoController> m_controller;
    std::vector<std::uint8_t>                 m_ids;
};
}
