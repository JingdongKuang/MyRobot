#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "MockTransport.h"
#include "ServoController.h"

int main()
{
	using myservo::MockTransport;
	using myservo::ServoController;
	constexpr std::uint16_t kMidRangeTestPosition = 420;
	constexpr std::uint16_t kOutOfRangePosition = 1001;

auto transport = std::make_shared<MockTransport>();
ServoController servo(transport);

	assert(servo.Ping(1));
	servo.SetPosition(1, kMidRangeTestPosition);
	assert(servo.ReadPosition(1) == kMidRangeTestPosition);

bool threwOutOfRange = false;
	try
	{
		servo.SetPosition(1, kOutOfRangePosition);
}
catch (const std::out_of_range&)
{
threwOutOfRange = true;
}
assert(threwOutOfRange);

bool threwInvalidId = false;
try
{
servo.Ping(0);
}
catch (const std::invalid_argument&)
{
threwInvalidId = true;
}
assert(threwInvalidId);

std::cout << "ServoTests passed!" << std::endl;
return 0;
}
