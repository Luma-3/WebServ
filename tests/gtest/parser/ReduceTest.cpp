/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReduceTest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:19 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/18 09:31:41 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "parser/Parser.hpp"

using parser::Parser;

TEST(R1_Server, Simple)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("server", T_Server, 1, 1));
	stack_test.push(new Token("{", T_OBracket, 1, 2));
	stack_test.push(new Param("listen", "129.0.0.1", "8080"));
	stack_test.push(new Token("}", T_CBracket, 4, 1));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R1_Server());

	EXPECT_NE(parser.getServers().at(0), nullptr);
	EXPECT_EQ(parser.getServers().at(0)->getParamPair("listen").first,
			  "129.0.0.1");

	EXPECT_EQ(parser.getServers().at(0)->getParamValue("hostname"),
			  "129.0.0.1:8080");
}

TEST(R1_Server, Two_params)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("server", T_Server, 1, 1));
	stack_test.push(new Token("{", T_OBracket, 1, 2));
	stack_test.push(new Param("listen", "210.3.12.2", "8080"));
	stack_test.push(new Param("hostname", "localhost"));
	stack_test.push(new Token("}", T_CBracket, 4, 1));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R1_Server());

	EXPECT_NE(parser.getServers().at(0), nullptr);
	EXPECT_EQ(parser.getServers().at(0)->getParamPair("listen").first,
			  "210.3.12.2");
	EXPECT_EQ(parser.getServers().at(0)->getParamValue("hostname"),
			  "localhost");
}

TEST(R1_Server, Same_param)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("server", T_Server, 1, 1));
	stack_test.push(new Token("{", T_OBracket, 1, 2));
	stack_test.push(new Param("listen", "127.0.0.1", "8080"));
	stack_test.push(new Param("listen", "localhost", "8081"));
	stack_test.push(new Token("}", T_CBracket, 4, 1));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R1_Server());

	EXPECT_NE(parser.getServers().at(0), nullptr);

	EXPECT_EQ(parser.getServers().at(0)->getParamPair("listen").first,
			  "localhost");
	EXPECT_NE(parser.getServers().at(0)->getParamPair("listen").second, "8080");
}

TEST(R1_Server, Missing_param)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("server", T_Server, 1, 1));
	stack_test.push(new Token("{", T_OBracket, 1, 2));
	stack_test.push(new Param("hostname", "localhost"));
	stack_test.push(new Token("}", T_CBracket, 4, 1));

	parser.setParseStack(stack_test);

	ASSERT_THROW(parser.R1_Server(), Parser::MissingParamException);
}

TEST(R1_Server, All_param)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("server", T_Server, 1, 1));
	stack_test.push(new Token("{", T_OBracket, 1, 2));
	stack_test.push(new Param("listen", "localhost", "8080"));
	stack_test.push(new Param("hostname", "localhost"));
	stack_test.push(new Param("root", "/var/www/html"));
	stack_test.push(new Param("index", "index.html"));
	stack_test.push(new Param("404", "/404.html"));
	stack_test.push(new Param("autoindex", "on"));
	stack_test.push(new Param("php", "/usr/bin/php-cgi"));
	std::vector< std::string > methods = {"GET", "POST"};
	stack_test.push(new Param("deny_method", methods));
	stack_test.push(new Token("}", T_CBracket, 4, 1));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R1_Server());

	EXPECT_NE(parser.getServers().at(0), nullptr);

	EXPECT_EQ(parser.getServers().at(0)->getParamPair("listen").first,
			  "localhost");
	EXPECT_EQ(parser.getServers().at(0)->getParamValue("hostname"),
			  "localhost");
	EXPECT_EQ(parser.getServers().at(0)->getParamValue("root"),
			  "/var/www/html");
	EXPECT_EQ(parser.getServers().at(0)->getParamValue("index"), "index.html");
	EXPECT_EQ(parser.getServers().at(0)->getParamValue("404"), "/404.html");
	EXPECT_EQ(parser.getServers().at(0)->getParamValue("autoindex"), "on");
	EXPECT_EQ(parser.getServers().at(0)->getParamValue("php"),
			  "/usr/bin/php-cgi");
	EXPECT_EQ(parser.getServers().at(0)->getParamList("deny_method").size(),
			  (size_t)2);
	EXPECT_EQ(parser.getServers().at(0)->getParamList("deny_method").at(0),
			  "GET");
	EXPECT_EQ(parser.getServers().at(0)->getParamList("deny_method").at(1),
			  "POST");
}

std::string get_castParm(IParserToken *token)
{
	Param *param = dynamic_cast< Param * >(token);
	return param ? param->getValue() : "";
}

TEST(R2_Param, Root)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("root", T_Root, 1, 1));
	stack_test.push(new Token("/", T_Path, 1, 2));
	stack_test.push(new Token(";", T_SemiColon, 1, 3));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R2_Param());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "root");
	EXPECT_EQ(get_castParm(parser.getParseStack().top()), "/");
}

TEST(R2_Param, Index)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("index", T_Index, 1, 1));
	stack_test.push(new Token("index.html", T_Identifier, 1, 2));
	stack_test.push(new Token(";", T_SemiColon, 1, 3));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R2_Param());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "index");
	EXPECT_EQ(get_castParm(parser.getParseStack().top()), "index.html");
}

TEST(R2_Param, Autoindex)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("autoindex", T_AutoIndex, 1, 1));
	stack_test.push(new Token("on", T_Bool, 1, 2));
	stack_test.push(new Token(";", T_SemiColon, 1, 3));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R2_Param());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "autoindex");
	EXPECT_EQ(get_castParm(parser.getParseStack().top()), "on");
}

TEST(R2_Param, InvalidHostname)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("hostname", T_Hostname, 1, 1));
	stack_test.push(new Token("localhost_", T_Identifier, 1, 2));
	stack_test.push(new Token(";", T_SemiColon, 1, 3));

	parser.setParseStack(stack_test);

	ASSERT_THROW(parser.R2_Param(), Token::InvalidTokenException);
}

TEST(R2_Param, InvalidBodySize)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("max_body_size", T_BodySize, 1, 1));
	stack_test.push(new Token("1p", T_Identifier, 1, 2));
	stack_test.push(new Token(";", T_SemiColon, 1, 3));

	parser.setParseStack(stack_test);

	ASSERT_THROW(parser.R2_Param(), Token::InvalidTokenException);
}

std::pair< std::string, std::string > get_castPair(IParserToken *token)
{
	Param *param = dynamic_cast< Param * >(token);
	return param ? param->getPair() : std::pair< std::string, std::string >();
}

TEST(R3_DoubleParam, Listen)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("listen", T_Listen, 1, 1));
	stack_test.push(new Token("localhost", T_Identifier, 1, 2));
	stack_test.push(new Token(":", T_Colon, 1, 3));
	stack_test.push(new Token("8080", T_Digits, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R3_DoubleParam());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "listen");
	EXPECT_EQ(get_castPair(parser.getParseStack().top()).first, "localhost");
	EXPECT_EQ(get_castPair(parser.getParseStack().top()).second, "8080");
}

TEST(R3_DoubleParam, Return)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("return", T_Return, 1, 1));
	stack_test.push(new Token("404", T_Digits, 1, 2));
	stack_test.push(new Token("https://www.google.com", T_Identifier, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R3_DoubleParam());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "return");
	EXPECT_EQ(get_castPair(parser.getParseStack().top()).first, "404");
	EXPECT_EQ(get_castPair(parser.getParseStack().top()).second,
			  "https://www.google.com");
}

TEST(R3_DoubleParam, InvalidPort)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("listen", T_Listen, 1, 1));
	stack_test.push(new Token("localhost", T_Identifier, 1, 2));
	stack_test.push(new Token("808p0", T_Identifier, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_THROW(parser.R3_DoubleParam(), Token::InvalidTokenException);
}

TEST(R3_DoubleParam, InvalidHostname)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("listen", T_Listen, 1, 1));
	stack_test.push(new Token("localhost_", T_Identifier, 1, 2));
	stack_test.push(new Token("8080", T_Digits, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_THROW(parser.R3_DoubleParam(), Token::InvalidTokenException);
}

TEST(R3_DoubleParam, InvalidErrorCode)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("return", T_Return, 1, 1));
	stack_test.push(new Token("4040", T_Digits, 1, 2));
	stack_test.push(new Token("https://www.google.com", T_Identifier, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_THROW(parser.R3_DoubleParam(), Token::InvalidTokenException);
}

TEST(R4_ErrorPage, One_Param)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("error_page", T_ErrorPage, 1, 1));
	stack_test.push(new Token("[", T_OSquareBracket, 1, 2));
	stack_test.push(new Token("404", T_Digits, 1, 2));
	stack_test.push(new Token("]", T_CSquareBracket, 1, 3));
	stack_test.push(new Token("/404.html", T_Identifier, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R4_ErrorPage());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "404");
	EXPECT_EQ(get_castParm(parser.getParseStack().top()), "/404.html");
}

TEST(R4_ErrorPage, Two_Params)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("error_page", T_ErrorPage, 1, 1));
	stack_test.push(new Token("[", T_OSquareBracket, 1, 2));
	stack_test.push(new Token("404", T_Digits, 1, 2));
	stack_test.push(new Token(",", T_Comma, 1, 3));
	stack_test.push(new Token("405", T_Digits, 1, 3));
	stack_test.push(new Token("]", T_CSquareBracket, 1, 3));
	stack_test.push(new Token("/404.html", T_Identifier, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 6));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R4_ErrorPage());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "405");
	EXPECT_EQ(get_castParm(parser.getParseStack().top()), "/404.html");

	parser.getParseStack().pop();

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "404");
	EXPECT_EQ(get_castParm(parser.getParseStack().top()), "/404.html");
}

TEST(R4_ErrorPage, InvalidErrorCode)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("error_page", T_ErrorPage, 1, 1));
	stack_test.push(new Token("[", T_OSquareBracket, 1, 2));
	stack_test.push(new Token("4040", T_Digits, 1, 2));
	stack_test.push(new Token("]", T_CSquareBracket, 1, 3));
	stack_test.push(new Token("/404.html", T_Identifier, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_THROW(parser.R4_ErrorPage(), Token::InvalidTokenException);
}

std::vector< std::string > get_castList(IParserToken *token)
{
	Param *param = dynamic_cast< Param * >(token);
	return param ? param->getList() : std::vector< std::string >();
}

TEST(R5_DenyMethod, One_Method)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("deny_method", T_DenyMethod, 1, 1));
	stack_test.push(new Token("[", T_OSquareBracket, 1, 2));
	stack_test.push(new Token("GET", T_Method, 1, 2));
	stack_test.push(new Token("]", T_CSquareBracket, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 3));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R5_DenyMethod());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "deny_method");
	EXPECT_EQ(get_castList(parser.getParseStack().top()).size(), (size_t)1);
	EXPECT_EQ(get_castList(parser.getParseStack().top()).at(0), "GET");
}

TEST(R5_DenyMethod, Two_Method)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("deny_method", T_DenyMethod, 1, 1));
	stack_test.push(new Token("[", T_OSquareBracket, 1, 2));
	stack_test.push(new Token("GET", T_Method, 1, 2));
	stack_test.push(new Token(",", T_Comma, 1, 3));
	stack_test.push(new Token("POST", T_Method, 1, 3));
	stack_test.push(new Token("]", T_CSquareBracket, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R5_DenyMethod());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "deny_method");
	EXPECT_EQ(get_castList(parser.getParseStack().top()).size(), (size_t)2);
	EXPECT_EQ(get_castList(parser.getParseStack().top()).at(0), "GET");
	EXPECT_EQ(get_castList(parser.getParseStack().top()).at(1), "POST");
}

TEST(R6_Location, Simple)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("location", T_Location, 1, 1));
	stack_test.push(new Token("/", T_Path, 1, 2));
	stack_test.push(new Token("{", T_OBracket, 1, 3));
	stack_test.push(new Token("}", T_CBracket, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R6_Location());

	const VirtualServer *server = parser.getCurrent();

	EXPECT_NE(server, nullptr);
	EXPECT_NE(server->getLocation("/"), nullptr);
}

TEST(R6_Location, WithOneParam)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("location", T_Location, 1, 1));
	stack_test.push(new Token("/", T_Path, 1, 2));
	stack_test.push(new Token("{", T_OBracket, 1, 3));
	stack_test.push(new Param("root", "/var/www/html"));
	stack_test.push(new Token("}", T_CBracket, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R6_Location());

	const VirtualServer *server = parser.getCurrent();

	EXPECT_NE(server, nullptr);
	EXPECT_NE(server->getLocation("/"), nullptr);
	EXPECT_EQ(server->getLocation("/")->getParamValue("root"), "/var/www/html");
}

TEST(R6_Location, WithTwoParam)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("location", T_Location, 1, 1));
	stack_test.push(new Token("/", T_Path, 1, 2));
	stack_test.push(new Token("{", T_OBracket, 1, 3));
	stack_test.push(new Param("root", "/var/www/html"));
	stack_test.push(new Param("index", "index.html"));
	stack_test.push(new Token("}", T_CBracket, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R6_Location());

	const VirtualServer *server = parser.getCurrent();

	EXPECT_NE(server, nullptr);
	EXPECT_NE(server->getLocation("/"), nullptr);
	EXPECT_EQ(server->getLocation("/")->getParamValue("root"), "/var/www/html");
	EXPECT_EQ(server->getLocation("/")->getParamValue("index"), "index.html");
}

TEST(R6_Location, WithSameParam)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("location", T_Location, 1, 1));
	stack_test.push(new Token("/", T_Path, 1, 2));
	stack_test.push(new Token("{", T_OBracket, 1, 3));
	stack_test.push(new Param("root", "/var/www/html"));
	stack_test.push(new Param("root", "/var/www/html2"));
	stack_test.push(new Param("autoindex", "on"));
	stack_test.push(new Token("}", T_CBracket, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R6_Location());

	const VirtualServer *server = parser.getCurrent();

	EXPECT_NE(server, nullptr);
	EXPECT_NE(server->getLocation("/"), nullptr);
	EXPECT_NE(server->getLocation("/")->getParamValue("root"), "/var/www/html");
	EXPECT_EQ(server->getLocation("/")->getParamValue("root"),
			  "/var/www/html2");
	EXPECT_EQ(server->getLocation("/")->getParamValue("autoindex"), "on");
}

TEST(R6_Location, WithAllParam)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("location", T_Location, 1, 1));
	stack_test.push(new Token("/", T_Path, 1, 2));
	stack_test.push(new Token("{", T_OBracket, 1, 3));
	stack_test.push(new Param("root", "/var/www/html"));
	stack_test.push(new Param("index", "index.html"));
	stack_test.push(new Param("404", "/404.html"));
	stack_test.push(new Param("autoindex", "on"));
	stack_test.push(new Param("php", "/usr/bin/php-cgi"));
	std::vector< std::string > methods = {"GET", "POST"};
	stack_test.push(new Param("deny_method", methods));
	stack_test.push(new Token("}", T_CBracket, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R6_Location());

	const VirtualServer *server = parser.getCurrent();

	EXPECT_NE(server, nullptr);
	EXPECT_NE(server->getLocation("/"), nullptr);
	EXPECT_EQ(server->getLocation("/")->getParamValue("root"), "/var/www/html");
	EXPECT_EQ(server->getLocation("/")->getParamValue("index"), "index.html");
	EXPECT_EQ(server->getLocation("/")->getParamValue("404"), "/404.html");
	EXPECT_EQ(server->getLocation("/")->getParamValue("autoindex"), "on");
	EXPECT_EQ(server->getLocation("/")->getParamValue("php"),
			  "/usr/bin/php-cgi");
	EXPECT_EQ(server->getLocation("/")->getParamList("deny_method").size(),
			  (size_t)2);
	EXPECT_EQ(server->getLocation("/")->getParamList("deny_method").at(0),
			  "GET");
	EXPECT_EQ(server->getLocation("/")->getParamList("deny_method").at(1),
			  "POST");
}

TEST(R7_CGI, PHP)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("cgi", T_CGI, 1, 1));
	stack_test.push(new Token("php", T_CGIExtension, 1, 2));
	stack_test.push(new Token("/usr/bin/php", T_Identifier, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R7_CGI());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "php");
	EXPECT_EQ(get_castParm(parser.getParseStack().top()), "/usr/bin/php");
}

TEST(R7_CGI, PY)
{
	Parser parser;

	std::stack< IParserToken * > stack_test;

	stack_test.push(new Token("cgi", T_CGI, 1, 1));
	stack_test.push(new Token("py", T_CGIExtension, 1, 2));
	stack_test.push(new Token("/usr/bin/python", T_Identifier, 1, 3));
	stack_test.push(new Token(";", T_SemiColon, 1, 4));

	parser.setParseStack(stack_test);

	ASSERT_NO_THROW(parser.R7_CGI());

	EXPECT_NE(parser.getParseStack().top(), nullptr);
	EXPECT_EQ(parser.getParseStack().top()->getKey(), "py");
	EXPECT_EQ(get_castParm(parser.getParseStack().top()), "/usr/bin/python");
}