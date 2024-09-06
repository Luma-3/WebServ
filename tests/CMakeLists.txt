enable_testing()

add_executable(exemple_test Test.cpp)

find_package(GTest REQUIRED PATHS ../externals/googletest/build/googletest/generated/)

# include_directories(../externals/googletest/googletest/include)

# add_dependencies(exemple_test gtest)

# target_link_libraries(exemple_test -L ../externals/googletest/build/lib)

add_test(NAME Test_name COMMAND exemple_test)