# robot_model 库：机器人机械参数管理（关节、连杆、URDF 导入等）
file(GLOB ROBOT_MODEL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB ROBOT_MODEL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_library(robot_model STATIC ${ROBOT_MODEL_SOURCES} ${ROBOT_MODEL_HEADERS})
target_include_directories(robot_model PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if (EIGEN3_INCLUDE_DIR)
  target_include_directories(robot_model PUBLIC "${EIGEN3_INCLUDE_DIR}")
endif()
