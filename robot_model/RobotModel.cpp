#include "RobotModel.h"

namespace myrobot
{
std::size_t RobotModel::DegreesOfFreedom() const noexcept
{
    std::size_t dof = 0;
    for (const auto& joint : m_joints)
    {
        if (joint.type != JointType::kFixed)
        {
            ++dof;
        }
    }
    return dof;
}

void RobotModel::Clear()
{
    m_name.clear();
    m_links.clear();
    m_joints.clear();
}
}
