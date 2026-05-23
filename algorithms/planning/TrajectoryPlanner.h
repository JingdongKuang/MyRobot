#pragma once

#include "Planner.h"

namespace myplanning
{
// 简单的线性插值轨迹规划器骨架。
// 当前仅声明接口，具体实现待补充。
class LinearTrajectoryPlanner : public IPlanner
{
public:
    explicit LinearTrajectoryPlanner(std::size_t numSegments = 10);

    JointTrajectory Plan(const JointWaypoint& start,
                         const JointWaypoint& goal) override;

private:
    std::size_t m_numSegments;
};
}
