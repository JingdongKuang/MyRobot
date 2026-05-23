file(GLOB SERVO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB SERVO_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_library(servo STATIC ${SERVO_SOURCES} ${SERVO_HEADERS})
target_include_directories(servo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if (BUILD_SERVO_TESTS)
  add_subdirectory("tests")
endif()
