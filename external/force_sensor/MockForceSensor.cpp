#include "MockForceSensor.h"

namespace myexternal
{
MockForceSensor::MockForceSensor(ForceReading constantReading)
    : m_reading(constantReading)
{
}

bool MockForceSensor::Open()
{
    m_isOpen = true;
    return true;
}

void MockForceSensor::Close()
{
    m_isOpen = false;
}

void MockForceSensor::Zero()
{
    m_reading = ForceReading{};
}

ForceReading MockForceSensor::Read()
{
    return m_reading;
}
}
