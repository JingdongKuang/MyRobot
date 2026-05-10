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

# 指定Eigen库的路径（必须在 add_subdirectory 之前）
if (NOT EIGEN3_INCLUDE_DIR)
  set(EIGEN3_INCLUDE_DIR "F:/externalLib/cpp/eigen-3.4.0")#此处不能用\,只能用/
endif()

# 链接 myfilter 库
add_subdirectory("myFilter")
add_subdirectory("servo")

if (BUILD_APP)
  # 将源代码添加到此项目的可执行文件。
  add_executable (MyRobot "MyRobot.cpp" "MyRobot.h")
  target_link_libraries(MyRobot PRIVATE myfilter)
  target_link_libraries(MyRobot PRIVATE servo)

  # 链接 math 库
  add_subdirectory("math")
  target_link_libraries(MyRobot PRIVATE math)

  if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET MyRobot PROPERTY CXX_STANDARD 20)
  endif()

  # 将Eigen库链接到项目中
  target_include_directories(MyRobot PRIVATE ${EIGEN3_INCLUDE_DIR})
endif()

if (BUILD_MYFILTER_TESTS OR BUILD_SERVO_TESTS)
  enable_testing()
endif()
