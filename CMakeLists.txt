cmake_minimum_required(VERSION 3.5)

project(WebServ CXX)

add_compile_options(-g3 -Wextra -Wall -Werror -Wunused -Wshadow)

include_directories(./includes)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(WebServ 
		sources/main.cpp
		sources/Client.cpp
		)

add_subdirectory(externals/googletest)

add_subdirectory(tests)