# myfilter ¿â
file(GLOB MYFILTER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB MYFILTER_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_library(myfilter STATIC ${MYFILTER_SOURCES} ${MYFILTER_HEADERS})
target_include_directories(myfilter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${EIGEN3_INCLUDE_DIR}")

if (MSVC)
  target_compile_options(myfilter PRIVATE /Z7)
endif()

if (BUILD_MYFILTER_TESTS)
  add_subdirectory("tests")
endif()

