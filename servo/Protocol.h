#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace myservo::protocol
{
	inline constexpr std::uint8_t kFrameHeader = 0xAA;
	inline constexpr std::size_t kMinFrameSize = 5;

	inline std::uint8_t Checksum(const std::vector<std::uint8_t>& frame)
	{
		std::uint8_t sum = 0;
		for (auto byte : frame)
		{
			sum = static_cast<std::uint8_t>(sum + byte);
		}
		return static_cast<std::uint8_t>(~sum);
	}
}
