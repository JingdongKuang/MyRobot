#pragma once

#include "Joint.h"
#include "Link.h"

#include <string>
#include <vector>

namespace myrobot
{
// 机器人整体机械模型：包含一组连杆与一组关节，外加全局属性。
class RobotModel
{
public:
    RobotModel() = default;

    void SetName(const std::string& name) { m_name = name; }
    const std::string& Name() const noexcept { return m_name; }

    void AddLink(const Link& link)   { m_links.push_back(link); }
    void AddJoint(const Joint& joint){ m_joints.push_back(joint); }

    const std::vector<Link>&  Links()  const noexcept { return m_links; }
    const std::vector<Joint>& Joints() const noexcept { return m_joints; }

    // 自由度 = 非固定关节数量。
    std::size_t DegreesOfFreedom() const noexcept;

    // 清空当前模型（在重新加载 URDF 等场景使用）。
    void Clear();

private:
    std::string        m_name;
    std::vector<Link>  m_links;
    std::vector<Joint> m_joints;
};
}
