#pragma once

#include "IForceSensor.h"

namespace myexternal
{
// 用于开发/测试的假力传感器：始终返回构造时设定的常量读数。
class MockForceSensor : public IForceSensor
{
public:
    MockForceSensor() = default;
    explicit MockForceSensor(ForceReading constantReading);

    bool Open() override;
    void Close() override;
    void Zero() override;
    ForceReading Read() override;

    void SetReading(const ForceReading& reading) { m_reading = reading; }

private:
    ForceReading m_reading{};
    bool         m_isOpen{ false };
};
}
