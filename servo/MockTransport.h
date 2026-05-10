#pragma once

#include "ServoController.h"

#include <unordered_map>

namespace myservo
{
class MockTransport final : public ITransport
{
public:
MockTransport() = default;
std::vector<std::uint8_t> Transfer(const std::vector<std::uint8_t>& request) override;

private:
std::uint8_t Checksum(const std::vector<std::uint8_t>& frame) const;
std::vector<std::uint8_t> BuildResponse(std::uint8_t id, Command cmd, const std::vector<std::uint8_t>& payload) const;

std::unordered_map<std::uint8_t, std::uint16_t> m_positions;
};
}
