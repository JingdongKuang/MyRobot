# CMakeList.txt: MyRobot 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MyRobot")

option(BUILD_APP "Build MyRobot executable" ON)
option(BUILD_MYFILTER_TESTS "Build myfilter tests" ON)
option(BUILD_SERVO_TESTS "Build servo tests" ON)
option(BUILD_ROBOT_MODEL "Build robot_model library" ON)
option(BUILD_ALGORITHMS "Build algorithms (planning + control) libraries" ON)
option(BUILD_EXTERNAL "Build external adapters (servo_interface + force_sensor)" ON)

if (BUILD_MYFILTER_TESTS OR BUILD_SERVO_TESTS)
  enable_testing()
endif()

# 指定Eigen库的路径（必须在 add_subdirectory 之前）
if (NOT EIGEN3_INCLUDE_DIR)
  set(EIGEN3_INCLUDE_DIR "F:/externalLib/cpp/eigen-3.4.0")#此处不能用\,只能用/
endif()

# 底层基础库
add_subdirectory("math")
add_subdirectory("algorithms/filter")
add_subdirectory("external/servo")

# 机器人机械模型（关节、连杆、URDF 导入等）
if (BUILD_ROBOT_MODEL OR BUILD_ALGORITHMS)
  add_subdirectory("robot_model")
endif()

# 算法层（规划 + 控制）
if (BUILD_ALGORITHMS)
  add_subdirectory("algorithms")
endif()

# 外部硬件对接层（伺服适配 + 力传感器）
if (BUILD_EXTERNAL)
  add_subdirectory("external")
endif()

if (BUILD_APP)
  # 将源代码添加到此项目的可执行文件。
  add_executable (MyRobot "MyRobot.cpp" "MyRobot.h")
  target_link_libraries(MyRobot PRIVATE myfilter)
  target_link_libraries(MyRobot PRIVATE servo)
  target_link_libraries(MyRobot PRIVATE math)

  if (TARGET robot_model)
    target_link_libraries(MyRobot PRIVATE robot_model)
  endif()
  if (TARGET planning)
    target_link_libraries(MyRobot PRIVATE planning)
  endif()
  if (TARGET control)
    target_link_libraries(MyRobot PRIVATE control)
  endif()
  if (TARGET servo_interface)
    target_link_libraries(MyRobot PRIVATE servo_interface)
  endif()
  if (TARGET force_sensor)
    target_link_libraries(MyRobot PRIVATE force_sensor)
  endif()

  if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET MyRobot PROPERTY CXX_STANDARD 20)
  endif()

  # 将Eigen库链接到项目中
  target_include_directories(MyRobot PRIVATE ${EIGEN3_INCLUDE_DIR})
endif()
