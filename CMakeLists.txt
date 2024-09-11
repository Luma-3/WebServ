cmake_minimum_required(VERSION 3.5)

project(WebServ CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Wextra -Wall -Werror -Wunused -Wshadow  -std=c++98")

include_directories(includes)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(WebServ 
	sources/main.cpp
	sources/parser/Parser.cpp
	sources/parser/tokenize.cpp
	sources/parser/Token.cpp
)

target_include_directories(WebServ PRIVATE includes)


add_subdirectory(externals/googletest)

add_subdirectory(tests)