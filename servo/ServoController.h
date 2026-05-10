#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace myservo
{
enum class Command : std::uint8_t
{
kPing = 0x01,
kSetPosition = 0x10,
kReadPosition = 0x11,
};

class ITransport
{
public:
virtual ~ITransport() = default;
virtual std::vector<std::uint8_t> Transfer(const std::vector<std::uint8_t>& request) = 0;
};

class ServoController
{
public:
explicit ServoController(std::shared_ptr<ITransport> transport);

bool Ping(std::uint8_t id) const;
void SetPosition(std::uint8_t id, std::uint16_t position) const;
std::uint16_t ReadPosition(std::uint8_t id) const;

private:
		std::vector<std::uint8_t> BuildFrame(std::uint8_t id, Command cmd, const std::vector<std::uint8_t>& payload) const;
		std::vector<std::uint8_t> Send(std::uint8_t id, Command cmd, const std::vector<std::uint8_t>& payload) const;
		void ValidateResponse(std::uint8_t expectedId, Command expectedCmd, const std::vector<std::uint8_t>& response) const;

		std::shared_ptr<ITransport> m_transport;
	};
}
