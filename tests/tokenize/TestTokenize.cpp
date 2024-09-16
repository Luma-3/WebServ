/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestTokenize.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:49:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/16 14:59:56 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "parser/Lexer.hpp"
#include "parser/Parser.hpp"
#include "parser/Token.hpp"

TEST(Token, createToken)
{
	Token token("server", Key);

	EXPECT_EQ(token.getType(), Key);
	EXPECT_EQ(token.getValue(), "server");
}

TEST(Token, copyToken)
{
	Token token("server", Key);

	Token copy(token);

	EXPECT_EQ(copy.getType(), Key);
	EXPECT_EQ(copy.getValue(), "server");
}

TEST(Token, assignToken)
{
	Token token("server", Key);
	Token copy("location", Key);

	copy = token;

	EXPECT_EQ(copy.getType(), Key);
	EXPECT_EQ(copy.getValue(), "server");
}

TEST(Tokenize, createLexer)
{
	Lexer lexer("../../tests/tokenize/testNormal.conf");

	EXPECT_EQ(lexer.getConfigFile().is_open(), true);
}

TEST(Tokenize, TokenizeNormal)
{
	Lexer lexer("../../tests/tokenize/testNormal.conf");

	lexer.Tokenize();

	std::vector< Token * > tokens = lexer.getTokens();

	EXPECT_EQ(tokens.size(), 7);
	EXPECT_EQ(tokens[0]->getValue(), "server");
	EXPECT_EQ(tokens[1]->getValue(), "{");
	EXPECT_EQ(tokens[2]->getValue(), "listen");
	EXPECT_EQ(tokens[3]->getValue(), "=");
	EXPECT_EQ(tokens[4]->getValue(), "8080");
	EXPECT_EQ(tokens[5]->getValue(), ";");
	EXPECT_EQ(tokens[6]->getValue(), "}");
}

TEST(Tokenize, TokenizeEmpty)
{
	Lexer lexer("../../tests/tokenize/testEmpty.conf");

	lexer.Tokenize();

	std::vector< Token * > tokens = lexer.getTokens();

	EXPECT_EQ(tokens.size(), 0);
}

TEST(Tokenize, TokenizeOneLine)
{
	Lexer lexer("../../tests/tokenize/testOneLine.conf");

	lexer.Tokenize();

	std::vector< Token * > tokens = lexer.getTokens();

	EXPECT_EQ(tokens.size(), 15);
	EXPECT_EQ(tokens[0]->getValue(), "server");
	EXPECT_EQ(tokens[1]->getValue(), "{");
	EXPECT_EQ(tokens[2]->getValue(), "listen");
	EXPECT_EQ(tokens[3]->getValue(), "=");
	EXPECT_EQ(tokens[4]->getValue(), "8080");
	EXPECT_EQ(tokens[5]->getValue(), ";");
	EXPECT_EQ(tokens[6]->getValue(), "location");
	EXPECT_EQ(tokens[7]->getValue(), "/");
	EXPECT_EQ(tokens[8]->getValue(), "{");
	EXPECT_EQ(tokens[9]->getValue(), "root");
	EXPECT_EQ(tokens[10]->getValue(), "=");
	EXPECT_EQ(tokens[11]->getValue(), "/var/www/html");
	EXPECT_EQ(tokens[12]->getValue(), ";");
	EXPECT_EQ(tokens[13]->getValue(), "}");
	EXPECT_EQ(tokens[14]->getValue(), "}");
}

TEST(Tokenize, TokenizeNotDotConf)
{
	EXPECT_THROW(Lexer lexer("../../tests/tokenize/testNotDotConf"),
				 Lexer::InvalidFileExtensionException);
}

int main(int ac, char **av)
{
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}