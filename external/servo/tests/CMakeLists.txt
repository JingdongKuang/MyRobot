add_executable(ServoTests "servoTests.cpp")

target_link_libraries(ServoTests PRIVATE servo)

add_test(NAME ServoTests COMMAND ServoTests)
