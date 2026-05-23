# planning 库：负责轨迹/路径规划等算法
file(GLOB PLANNING_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB PLANNING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_library(planning STATIC ${PLANNING_SOURCES} ${PLANNING_HEADERS})
target_include_directories(planning PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# 规划通常会用到机器人模型与底层数学库
if (TARGET robot_model)
  target_link_libraries(planning PUBLIC robot_model)
endif()
if (TARGET math)
  target_link_libraries(planning PUBLIC math)
endif()
