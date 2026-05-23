#include "URDFLoader.h"

namespace myrobot
{
bool URDFLoader::LoadFromFile(const std::string& /*path*/, RobotModel& /*model*/)
{
    // TODO: 读取文件内容并转交 LoadFromString。
    m_lastError = "URDFLoader::LoadFromFile not implemented yet";
    return false;
}

bool URDFLoader::LoadFromString(const std::string& /*urdfXml*/, RobotModel& /*model*/)
{
    // TODO: 解析 URDF XML（建议引入 tinyxml2 或 urdfdom），
    // 然后填充 model 的 Links / Joints / 全局属性。
    m_lastError = "URDFLoader::LoadFromString not implemented yet";
    return false;
}
}
