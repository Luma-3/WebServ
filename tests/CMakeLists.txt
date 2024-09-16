enable_testing()

add_executable(TokenizeTest tokenize/TestTokenize.cpp
							../sources/parser/Lexer.cpp
							../sources/parser/Token.cpp)

target_link_libraries(TokenizeTest gtest gtest_main)

add_test(NAME Tokenize_Test COMMAND ./TokenizeTest)