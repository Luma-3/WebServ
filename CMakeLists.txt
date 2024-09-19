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
	sources/lexer/LexerException.cpp
	sources/lexer/Lexer.cpp
	sources/lexer/regex.cpp
	sources/lexer/Token.cpp
	sources/parser/Parser.cpp
	sources/parser/Action.cpp
	sources/parser/statement/DenyMethod.cpp
	sources/parser/statement/Location.cpp
	sources/parser/statement/ErrorPage.cpp
	sources/parser/statement/Param.cpp
	sources/parser/statement/ReturnParam.cpp
	sources/parser/statement/Server.cpp

)

target_include_directories(WebServ PRIVATE includes)



add_subdirectory(tests)