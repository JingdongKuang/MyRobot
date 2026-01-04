add_executable(MyFilterTests "filterTests.cpp")

target_link_libraries(MyFilterTests PRIVATE myfilter)

add_test(NAME MyFilterTests COMMAND MyFilterTests)
