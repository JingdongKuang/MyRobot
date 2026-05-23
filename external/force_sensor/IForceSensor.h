#pragma once

#include "ForceReading.h"

namespace myexternal
{
// 力/力矩传感器抽象接口。任何具体硬件驱动（串口、CAN、EtherCAT 等）
// 都通过实现该接口接入系统。
class IForceSensor
{
public:
    virtual ~IForceSensor() = default;

    // 打开/初始化传感器。返回是否成功。
    virtual bool Open() = 0;

    // 关闭传感器并释放资源。
    virtual void Close() = 0;

    // 将当前读数置零（去重力补偿、安装零点标定等）。
    virtual void Zero() = 0;

    // 读取最新一帧力/力矩数据。
    virtual ForceReading Read() = 0;
};
}
