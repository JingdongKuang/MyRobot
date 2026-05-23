# control 库：负责闭环控制算法（PID、阻抗、力控等）
file(GLOB CONTROL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB CONTROL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_library(control STATIC ${CONTROL_SOURCES} ${CONTROL_HEADERS})
target_include_directories(control PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if (TARGET robot_model)
  target_link_libraries(control PUBLIC robot_model)
endif()
if (TARGET math)
  target_link_libraries(control PUBLIC math)
endif()
