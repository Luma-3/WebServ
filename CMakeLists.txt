cmake_minimum_required(VERSION 3.5)

project(WebServ CXX)

add_subdirectory(externals/googletest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Wextra -Wall -Werror -Wunused -Wshadow")

include_directories(includes)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(WebServ 
	sources/main.cpp
	sources/parser/Parser.cpp
	sources/parser/tokenize.cpp
	sources/parser/Token.cpp

)

target_include_directories(WebServ PRIVATE includes)



add_subdirectory(tests)