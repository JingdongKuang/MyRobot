# math ¿â
file(GLOB MATH_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB MATH_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

if(MATH_SOURCES)
  add_library(math STATIC ${MATH_SOURCES} ${MATH_HEADERS})
  target_include_directories(math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
else()
  add_library(math INTERFACE)
  target_include_directories(math INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
endif()