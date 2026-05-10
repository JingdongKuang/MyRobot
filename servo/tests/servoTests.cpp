#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "MockTransport.h"
#include "ServoController.h"

int main()
{
	using myservo::MockTransport;
	using myservo::ServoController;
	constexpr std::uint16_t kTestPosition = 420;

	auto transport = std::make_shared<MockTransport>();
	ServoController servo(transport);

	assert(servo.Ping(1));
	servo.SetPosition(1, kTestPosition);
	assert(servo.ReadPosition(1) == kTestPosition);

bool threwOutOfRange = false;
try
{
servo.SetPosition(1, 1001);
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
