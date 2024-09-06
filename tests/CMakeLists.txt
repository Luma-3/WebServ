enable_testing()

add_executable(exemple_test Test.cpp)

# include_directories(../externals/googletest/googletest/include)

# add_dependencies(exemple_test gtest)

target_link_libraries(exemple_test gtest gtest_main)

add_test(NAME Test_name COMMAND exemple_test)