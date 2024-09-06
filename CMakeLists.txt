cmake_minimum_required(VERSION 3.5)

project(WebServ CXX)

add_compile_options(-g3 -Wextra -Wall -Werror -Wunused -Wshadow)

include_directories(./includes)

add_executable(WebServ 
		sources/main.cpp
		)

execute_process(COMMAND mkdir -p externals/googletest/build
COMMAND cmake -S externals/googletest -B externals/googletest/build)
execute_process(COMMAND cmake --build externals/googletest/build)

add_subdirectory(tests)