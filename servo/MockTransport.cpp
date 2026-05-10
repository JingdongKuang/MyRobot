#include "MockTransport.h"
#include "Protocol.h"

#include <stdexcept>

namespace myservo
{
	std::vector<std::uint8_t> MockTransport::Transfer(const std::vector<std::uint8_t>& request)
	{
		if (request.size() < protocol::kMinFrameSize)
		{
			throw std::runtime_error("request too short");
		}
		if (request[0] != protocol::kFrameHeader)
		{
			throw std::runtime_error("invalid request header");
		}

		const auto payloadLength = static_cast<std::size_t>(request[3]);
		const auto expectedSize = protocol::kMinFrameSize + payloadLength;
		if (request.size() != expectedSize)
		{
			throw std::runtime_error("request size mismatch");
		}

		std::vector<std::uint8_t> withoutChecksum(request.begin(), request.end() - 1);
		if (request.back() != protocol::Checksum(withoutChecksum))
		{
			throw std::runtime_error("request checksum mismatch");
		}

const auto id = request[1];
const auto cmd = static_cast<Command>(request[2]);
std::vector<std::uint8_t> payload;
switch (cmd)
{
case Command::kPing:
break;
case Command::kSetPosition:
if (payloadLength != 2)
{
throw std::runtime_error("invalid set-position payload");
}
m_positions[id] = static_cast<std::uint16_t>((static_cast<std::uint16_t>(request[5]) << 8) | request[4]);
break;
case Command::kReadPosition:
{
const auto value = m_positions[id];
payload.push_back(static_cast<std::uint8_t>(value & 0xFF));
payload.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
break;
}
default:
throw std::runtime_error("unsupported command");
}

		return BuildResponse(id, cmd, payload);
	}

	std::vector<std::uint8_t> MockTransport::BuildResponse(std::uint8_t id, Command cmd, const std::vector<std::uint8_t>& payload) const
	{
		std::vector<std::uint8_t> response;
		response.reserve(protocol::kMinFrameSize + payload.size());
		response.push_back(protocol::kFrameHeader);
		response.push_back(id);
		response.push_back(static_cast<std::uint8_t>(cmd));
		response.push_back(static_cast<std::uint8_t>(payload.size()));
		response.insert(response.end(), payload.begin(), payload.end());
		response.push_back(protocol::Checksum(response));
		return response;
	}
}
