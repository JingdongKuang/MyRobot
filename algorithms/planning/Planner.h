#pragma once

#include <vector>

namespace myplanning
{
// 关节空间路点：每个元素表示一个关节的目标角度（弧度）。
using JointWaypoint = std::vector<double>;

// 规划结果：一系列关节空间路点构成的轨迹。
struct JointTrajectory
{
    std::vector<JointWaypoint> points;
};

// 规划器抽象接口。后续可派生出关节空间规划、笛卡尔空间规划等具体实现。
class IPlanner
{
public:
    virtual ~IPlanner() = default;

    // 给定起点与终点关节配置，输出一条可执行的关节轨迹。
    virtual JointTrajectory Plan(const JointWaypoint& start,
                                 const JointWaypoint& goal) = 0;
};
}
