enable_testing()

add_executable(TokenizeTest tokenize/TestTokenize.cpp
							../sources/lexer/LexerException.cpp
							../sources/lexer/Lexer.cpp
							../sources/lexer/Token.cpp
							../sources/lexer/regex.cpp)

target_link_libraries(TokenizeTest gtest gtest_main)

add_test(NAME Tokenize_Test COMMAND ./TokenizeTest)

add_executable(ParserTest	parser/TestParser.cpp
							../sources/parser/Parser.cpp
							../sources/parser/Action.cpp
							../sources/parser/ParserException.cpp
							../sources/parser/statement/DenyMethod.cpp
							../sources/parser/statement/ErrorPage.cpp
							../sources/parser/statement/Location.cpp
							../sources/parser/statement/Param.cpp
							../sources/parser/statement/ReturnParam.cpp
							../sources/parser/statement/Server.cpp
							../sources/lexer/Token.cpp
							../sources/lexer/regex.cpp
)

target_link_libraries(ParserTest gtest gtest_main)

add_test(NAME ParserTest COMMAND ./ParserTest)