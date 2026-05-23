cmake_minimum_required(VERSION 3.8)
project(myrobot_ros2)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(robot_node
  src/RobotNode.cpp
  src/SimServoAdapter.cpp
  src/SimForceSensorAdapter.cpp
)

if(TARGET planning)
  target_link_libraries(robot_node PRIVATE planning)
endif()
if(TARGET control)
  target_link_libraries(robot_node PRIVATE control)
endif()
if(TARGET servo_interface)
  target_link_libraries(robot_node PRIVATE servo_interface)
endif()
if(TARGET force_sensor)
  target_link_libraries(robot_node PRIVATE force_sensor)
endif()
if(TARGET servo)
  target_link_libraries(robot_node PRIVATE servo)
endif()

target_include_directories(robot_node PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../algorithms/planning"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../algorithms/control"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../external/servo_interface"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../external/force_sensor"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../external/servo"
)

ament_target_dependencies(robot_node
  rclcpp
  sensor_msgs
  geometry_msgs
)

install(TARGETS robot_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
