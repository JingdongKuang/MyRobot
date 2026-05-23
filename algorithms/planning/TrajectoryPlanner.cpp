#include "TrajectoryPlanner.h"

#include <stdexcept>

namespace myplanning
{
LinearTrajectoryPlanner::LinearTrajectoryPlanner(std::size_t numSegments)
    : m_numSegments(numSegments)
{
}

JointTrajectory LinearTrajectoryPlanner::Plan(const JointWaypoint& start,
                                              const JointWaypoint& goal)
{
    if (start.size() != goal.size())
    {
        throw std::invalid_argument("start and goal must have the same dof");
    }

    JointTrajectory trajectory;
    // TODO: 在此处实现线性插值生成轨迹点。
    // 当前先返回包含起点与终点的最小可用轨迹，便于上层调用接口。
    trajectory.points.push_back(start);
    trajectory.points.push_back(goal);
    return trajectory;
}
}
