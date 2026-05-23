# servo_interface 库：对接伺服硬件的上层适配层
# 在底层 servo 库（协议/通信）之上，提供面向机器人控制的多关节调用。
file(GLOB SERVO_IF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB SERVO_IF_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_library(servo_interface STATIC ${SERVO_IF_SOURCES} ${SERVO_IF_HEADERS})
target_include_directories(servo_interface PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# 依赖底层 servo 库（协议、ServoController、ITransport 等）。
if (TARGET servo)
  target_link_libraries(servo_interface PUBLIC servo)
endif()
