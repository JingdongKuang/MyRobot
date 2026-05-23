#pragma once

#include "RobotModel.h"

#include <string>

namespace myrobot
{
// URDF 文件加载器：把 URDF (XML) 解析为 RobotModel。
// 当前为骨架声明，具体 XML 解析待接入第三方库（如 tinyxml2 / urdfdom）后实现。
class URDFLoader
{
public:
    URDFLoader() = default;

    // 从文件路径加载。成功返回 true 并填充 model；失败返回 false。
    bool LoadFromFile(const std::string& path, RobotModel& model);

    // 从已经读入内存的 URDF 字符串加载。
    bool LoadFromString(const std::string& urdfXml, RobotModel& model);

    // 最近一次加载失败的可读错误信息。
    const std::string& LastError() const noexcept { return m_lastError; }

private:
    std::string m_lastError;
};
}
