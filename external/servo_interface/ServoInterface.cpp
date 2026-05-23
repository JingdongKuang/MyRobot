#include "ServoInterface.h"

#include "ServoController.h"

#include <stdexcept>
#include <utility>

namespace myexternal
{
ServoInterface::ServoInterface(std::shared_ptr<myservo::ServoController> controller,
                               std::vector<std::uint8_t> ids)
    : m_controller(std::move(controller)), m_ids(std::move(ids))
{
    if (!m_controller)
    {
        throw std::invalid_argument("controller must not be null");
    }
}

bool ServoInterface::PingAll() const
{
    for (auto id : m_ids)
    {
        if (!m_controller->Ping(id))
        {
            return false;
        }
    }
    return true;
}

void ServoInterface::SetJointPositions(const std::vector<std::uint16_t>& positions) const
{
    if (positions.size() != m_ids.size())
    {
        throw std::invalid_argument("positions size mismatch with joint count");
    }
    for (std::size_t i = 0; i < m_ids.size(); ++i)
    {
        m_controller->SetPosition(m_ids[i], positions[i]);
    }
}

std::vector<std::uint16_t> ServoInterface::ReadJointPositions() const
{
    std::vector<std::uint16_t> result;
    result.reserve(m_ids.size());
    for (auto id : m_ids)
    {
        result.push_back(m_controller->ReadPosition(id));
    }
    return result;
}
}
