/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:37:59 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/07 14:47:59 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "lexer/Token.hpp"
#include "parser/Location.hpp"
#include "parser/Param.hpp"
#include "parser/Parser.hpp"
#include "parser/ReturnParam.hpp"
#include "parser/Server.hpp"

using parser::Parser;

TEST(Parser, createParser)
{
	std::vector< Token * > tokens;

	Parser parser(tokens);

	EXPECT_EQ(parser.getState(), 0);
	EXPECT_EQ(parser.getTokens().size(), 0);
	EXPECT_EQ(parser.getParseStack().size(), 0);
}

TEST(Parser, setState)
{
	std::vector< Token * > tokens;

	Parser parser(tokens);

	parser.setState(42);

	EXPECT_EQ(parser.getState(), 42);
}

TEST(Parser, parseWithNothing)
{
	std::vector< Token * > tokens;

	Parser parser(tokens);

	EXPECT_NO_THROW(parser.Parse());
}

TEST(Parser, parseWithOnlyServerBlock)
{
	std::vector< Token * > tokens;

	tokens.push_back(new Token("server", T_Server, 0, 0));
	tokens.push_back(new Token("{", T_OBracket, 0, 0));
	tokens.push_back(new Token("}", T_CBracket, 0, 0));
	Parser parser(tokens);

	// -- Expected

	std::vector< Token * > server_tokens;
	statement::Server	   server(server_tokens);

	EXPECT_NO_THROW(parser.Parse());
	statement::Server *out =
		dynamic_cast< statement::Server * >(parser.getParseStack().top());
	EXPECT_EQ(*out, server);
}

TEST(Parser, parseWithOnlyServerBlockAndLocationBlock)
{
	std::vector< Token * > tokens;
	tokens.push_back(new Token("server", T_Server, 0, 0));
	tokens.push_back(new Token("{", T_OBracket, 0, 0));
	tokens.push_back(new Token("location", T_Location, 0, 0));
	tokens.push_back(new Token("/", T_Path, 0, 0));
	tokens.push_back(new Token("{", T_OBracket, 0, 0));
	tokens.push_back(new Token("}", T_CBracket, 0, 0));
	tokens.push_back(new Token("}", T_CBracket, 0, 0));
	Parser parser(tokens);

	// -- Expected

	std::vector< Token * > location_tokens;

	std::vector< Token * > server_tokens;
	server_tokens.push_back(new statement::Location(location_tokens, "/"));

	statement::Server server(server_tokens);

	EXPECT_NO_THROW(parser.Parse());
	statement::Server *out =
		dynamic_cast< statement::Server * >(parser.getParseStack().top());
	EXPECT_EQ(*out, server);
}

TEST(Parser, parseWithOnlyServerInfo)
{
	std::vector< Token * > tokens;
	tokens.push_back(new Token("server", T_Server, 0, 0));
	tokens.push_back(new Token("{", T_OBracket, 0, 0));
	tokens.push_back(new Token("host", T_Host, 0, 0));
	tokens.push_back(new Token("=", T_Equal, 0, 0));
	tokens.push_back(new Token("@localhost", T_HostValue, 0, 0));
	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
	tokens.push_back(new Token("port", T_Port, 0, 0));
	tokens.push_back(new Token("=", T_Equal, 0, 0));
	tokens.push_back(new Token("8080", T_PortValue, 0, 0));
	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
	tokens.push_back(new Token("}", T_CBracket, 0, 0));
	Parser parser(tokens);

	// -- Expected

	std::vector< Token * > server_tokens;
	server_tokens.push_back(new statement::Param("@localhost", T_Host));
	server_tokens.push_back(new statement::Param("8080", T_Port));

	statement::Server server(server_tokens);

	EXPECT_NO_THROW(parser.Parse());
	statement::Server *out =
		dynamic_cast< statement::Server * >(parser.getParseStack().top());
	EXPECT_EQ(*out, server);
}

// TEST(Parser, parseWithOnlyLocationInfo)
// {
// 	std::vector< Token * > tokens;
// 	tokens.push_back(new Token("server", T_Server, 0, 0));
// 	tokens.push_back(new Token("{", T_OBracket, 0, 0));
// 	tokens.push_back(new Token("location", T_Location, 0, 0));
// 	tokens.push_back(new Token("/", T_Path, 0, 0));
// 	tokens.push_back(new Token("{", T_OBracket, 0, 0));
// 	tokens.push_back(new Token("root", T_Root, 0, 0));
// 	tokens.push_back(new Token("=", T_Equal, 0, 0));
// 	tokens.push_back(new Token("/var/www/html/", T_Path, 0, 0));
// 	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
// 	tokens.push_back(new Token("index", T_Index, 0, 0));
// 	tokens.push_back(new Token("=", T_Equal, 0, 0));
// 	tokens.push_back(new Token("index.html", T_FileName, 0, 0));
// 	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
// 	tokens.push_back(new Token("autoindex", T_AutoIndex, 0, 0));
// 	tokens.push_back(new Token("=", T_Equal, 0, 0));
// 	tokens.push_back(new Token("on", T_Bool, 0, 0));
// 	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
// 	tokens.push_back(new Token("return", T_Return, 0, 0));
// 	tokens.push_back(new Token("=", T_Equal, 0, 0));
// 	tokens.push_back(new Token("404", T_ErrorCode, 0, 0));
// 	tokens.push_back(new Token(":", T_Colone, 0, 0));
// 	tokens.push_back(new Token("/404.html", T_FilePath, 0, 0));
// 	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
// 	tokens.push_back(new Token("}", T_CBracket, 0, 0));
// 	tokens.push_back(new Token("}", T_CBracket, 0, 0));
// 	Parser parser(tokens);

// 	// -- Expected

// 	std::vector< Token * > location_tokens;
// 	location_tokens.push_back(new statement::Param("/var/www/html/", T_Root));
// 	location_tokens.push_back(new statement::Param("index.html", T_Index));
// 	location_tokens.push_back(new statement::Param("on", T_AutoIndex));
// 	location_tokens.push_back(new statement::ReturnParam("404", "/404.html"));

// 	std::vector< Token * > server_tokens;
// 	server_tokens.push_back(new statement::Location(location_tokens, "/"));

// 	statement::Server server(server_tokens);

// 	EXPECT_NO_THROW(parser.Parse());
// 	statement::Server *out =
// 		dynamic_cast< statement::Server * >(parser.getParseStack().top());
// 	EXPECT_EQ(*out, server);
// }

TEST(Parser, parseWithExpectedError)
{
	std::vector< Token * > tokens;
	tokens.push_back(new Token("server", T_Server, 0, 0));
	tokens.push_back(new Token("{", T_OBracket, 0, 0));
	tokens.push_back(new Token("location", T_Location, 0, 0));
	tokens.push_back(new Token("/", T_Path, 0, 0));
	tokens.push_back(new Token("{", T_OBracket, 0, 0));
	tokens.push_back(new Token("root", T_Root, 0, 0));
	tokens.push_back(new Token("=", T_Equal, 0, 0));
	tokens.push_back(new Token("/var/www/html", T_Path, 0, 0));
	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
	tokens.push_back(new Token("index", T_Index, 0, 0));
	tokens.push_back(new Token("=", T_Equal, 0, 0));
	tokens.push_back(new Token("index.html", T_FileName, 0, 0));
	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
	tokens.push_back(new Token("autoindex", T_AutoIndex, 0, 0));
	tokens.push_back(new Token("=", T_Equal, 0, 0));
	tokens.push_back(new Token("on", T_Bool, 0, 0));
	tokens.push_back(new Token("return", T_Return, 0, 0));
	tokens.push_back(new Token("=", T_Equal, 0, 0));
	tokens.push_back(new Token("404", T_ErrorCode, 0, 0));
	tokens.push_back(new Token(":", T_Colone, 0, 0));
	tokens.push_back(new Token("/404.html", T_FilePath, 0, 0));
	tokens.push_back(new Token(";", T_Semi_Colon, 0, 0));
	tokens.push_back(new Token("}", T_CBracket, 0, 0));
	tokens.push_back(new Token("}", T_CBracket, 0, 0));
	Parser parser(tokens);

	// -- Expected

	std::vector< Token * > location_tokens;
	location_tokens.push_back(new statement::Param("/var/www/html", T_Root));
	location_tokens.push_back(new statement::Param("index.html", T_Index));
	location_tokens.push_back(new statement::Param("on", T_AutoIndex));
	location_tokens.push_back(new statement::ReturnParam("404", "/404.html"));

	std::vector< Token * > server_tokens;
	server_tokens.push_back(new statement::Location(location_tokens, "/"));

	statement::Server server(server_tokens);

	EXPECT_THROW(parser.Parse(), Parser::InvalidTokenException);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}