#include "ServoController.h"

#include <stdexcept>

namespace myservo
{
namespace
{
constexpr std::uint8_t kFrameHeader = 0xAA;
constexpr std::size_t kMinFrameSize = 5;
}

ServoController::ServoController(std::shared_ptr<ITransport> transport)
: m_transport(std::move(transport))
{
if (!m_transport)
{
throw std::invalid_argument("transport must not be null");
}
}

bool ServoController::Ping(std::uint8_t id) const
{
const auto response = Send(id, Command::kPing, {});
return !response.empty();
}

void ServoController::SetPosition(std::uint8_t id, std::uint16_t position) const
{
if (position > 1000)
{
throw std::out_of_range("position must be in [0,1000]");
}

std::vector<std::uint8_t> payload;
payload.push_back(static_cast<std::uint8_t>(position & 0xFF));
payload.push_back(static_cast<std::uint8_t>((position >> 8) & 0xFF));
Send(id, Command::kSetPosition, payload);
}

std::uint16_t ServoController::ReadPosition(std::uint8_t id) const
{
const auto response = Send(id, Command::kReadPosition, {});
const auto payloadLength = static_cast<std::size_t>(response[3]);
if (payloadLength != 2)
{
throw std::runtime_error("invalid read-position response payload length");
}

const std::uint16_t low = response[4];
const std::uint16_t high = response[5];
return static_cast<std::uint16_t>((high << 8) | low);
}

std::vector<std::uint8_t> ServoController::BuildFrame(std::uint8_t id, Command cmd, const std::vector<std::uint8_t>& payload) const
{
if (id == 0)
{
throw std::invalid_argument("servo id must be in [1,255]");
}
if (payload.size() > 255)
{
throw std::invalid_argument("payload too large");
}

std::vector<std::uint8_t> frame;
frame.reserve(kMinFrameSize + payload.size());
frame.push_back(kFrameHeader);
frame.push_back(id);
frame.push_back(static_cast<std::uint8_t>(cmd));
frame.push_back(static_cast<std::uint8_t>(payload.size()));
frame.insert(frame.end(), payload.begin(), payload.end());
frame.push_back(Checksum(frame));
return frame;
}

std::vector<std::uint8_t> ServoController::Send(std::uint8_t id, Command cmd, const std::vector<std::uint8_t>& payload) const
{
const auto request = BuildFrame(id, cmd, payload);
const auto response = m_transport->Transfer(request);
ValidateResponse(id, cmd, response);
return response;
}

void ServoController::ValidateResponse(std::uint8_t expectedId, Command expectedCmd, const std::vector<std::uint8_t>& response) const
{
if (response.size() < kMinFrameSize)
{
throw std::runtime_error("response too short");
}
if (response[0] != kFrameHeader)
{
throw std::runtime_error("invalid response header");
}
if (response[1] != expectedId)
{
throw std::runtime_error("response servo id mismatch");
}
if (response[2] != static_cast<std::uint8_t>(expectedCmd))
{
throw std::runtime_error("response command mismatch");
}

const auto payloadLength = static_cast<std::size_t>(response[3]);
const auto expectedSize = kMinFrameSize + payloadLength;
if (response.size() != expectedSize)
{
throw std::runtime_error("response size mismatch");
}

const auto checksum = response.back();
std::vector<std::uint8_t> withoutChecksum(response.begin(), response.end() - 1);
if (checksum != Checksum(withoutChecksum))
{
throw std::runtime_error("response checksum mismatch");
}
}

std::uint8_t ServoController::Checksum(const std::vector<std::uint8_t>& frame) const
{
std::uint8_t sum = 0;
for (auto byte : frame)
{
sum = static_cast<std::uint8_t>(sum + byte);
}
return static_cast<std::uint8_t>(~sum);
}
}
