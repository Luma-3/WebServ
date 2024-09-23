/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestTokenize.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:49:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/23 11:44:55 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"

TEST(Token, createToken)
{
	Token token("server", T_Server, 0, 0);

	EXPECT_EQ(token.getType(), S_Terminal);
	EXPECT_EQ(token.getValue(), "server");
	EXPECT_EQ(token.getTerminal(), T_Server);
	EXPECT_EQ(token.getLine(), 0);
	EXPECT_EQ(token.getCol(), 0);
}

TEST(Token, copyToken)
{
	Token token("server", T_Server, 0, 0);

	Token copy(token);

	EXPECT_EQ(copy.getType(), S_Terminal);
	EXPECT_EQ(copy.getValue(), "server");
	EXPECT_EQ(copy.getTerminal(), T_Server);
	EXPECT_EQ(copy.getLine(), 0);
	EXPECT_EQ(copy.getCol(), 0);
}

TEST(Token, assignToken)
{
	Token token("server", T_Server, 0, 0);
	Token copy("location", T_Location, 1, 1);

	copy = token;

	EXPECT_EQ(copy.getType(), S_Terminal);
	EXPECT_EQ(copy.getValue(), "server");
	EXPECT_EQ(copy.getTerminal(), T_Server);
	EXPECT_EQ(copy.getLine(), 0);
	EXPECT_EQ(copy.getCol(), 0);
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
	EXPECT_EQ(tokens[0]->getTerminal(), T_Server);
	EXPECT_EQ(tokens[1]->getValue(), "{");
	EXPECT_EQ(tokens[1]->getTerminal(), T_OBracket);
	EXPECT_EQ(tokens[2]->getValue(), "port");
	EXPECT_EQ(tokens[2]->getTerminal(), T_Port);
	EXPECT_EQ(tokens[3]->getValue(), "=");
	EXPECT_EQ(tokens[3]->getTerminal(), T_Equal);
	EXPECT_EQ(tokens[4]->getValue(), "8080");
	EXPECT_EQ(tokens[4]->getTerminal(), T_PortValue);
	EXPECT_EQ(tokens[5]->getValue(), ";");
	EXPECT_EQ(tokens[5]->getTerminal(), T_Semi_Colon);
	EXPECT_EQ(tokens[6]->getValue(), "}");
	EXPECT_EQ(tokens[6]->getTerminal(), T_CBracket);
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
	EXPECT_EQ(tokens[0]->getTerminal(), T_Server);
	EXPECT_EQ(tokens[1]->getValue(), "{");
	EXPECT_EQ(tokens[1]->getTerminal(), T_OBracket);
	EXPECT_EQ(tokens[2]->getValue(), "port");
	EXPECT_EQ(tokens[2]->getTerminal(), T_Port);
	EXPECT_EQ(tokens[3]->getValue(), "=");
	EXPECT_EQ(tokens[3]->getTerminal(), T_Equal);
	EXPECT_EQ(tokens[4]->getValue(), "8080");
	EXPECT_EQ(tokens[4]->getTerminal(), T_PortValue);
	EXPECT_EQ(tokens[5]->getValue(), ";");
	EXPECT_EQ(tokens[5]->getTerminal(), T_Semi_Colon);
	EXPECT_EQ(tokens[6]->getValue(), "location");
	EXPECT_EQ(tokens[6]->getTerminal(), T_Location);
	EXPECT_EQ(tokens[7]->getValue(), "/");
	EXPECT_EQ(tokens[7]->getTerminal(), T_Path);
	EXPECT_EQ(tokens[8]->getValue(), "{");
	EXPECT_EQ(tokens[8]->getTerminal(), T_OBracket);
	EXPECT_EQ(tokens[9]->getValue(), "root");
	EXPECT_EQ(tokens[9]->getTerminal(), T_Root);
	EXPECT_EQ(tokens[10]->getValue(), "=");
	EXPECT_EQ(tokens[10]->getTerminal(), T_Equal);
	EXPECT_EQ(tokens[11]->getValue(), "/var/www/html");
	EXPECT_EQ(tokens[11]->getTerminal(), T_FilePath);
	EXPECT_EQ(tokens[12]->getValue(), ";");
	EXPECT_EQ(tokens[12]->getTerminal(), T_Semi_Colon);
	EXPECT_EQ(tokens[13]->getValue(), "}");
	EXPECT_EQ(tokens[13]->getTerminal(), T_CBracket);
	EXPECT_EQ(tokens[14]->getValue(), "}");
	EXPECT_EQ(tokens[14]->getTerminal(), T_CBracket);
}

TEST(Tokenize, TokenizeSQuote)
{
	Lexer lexer("../../tests/tokenize/testSQuote.conf");

	lexer.Tokenize();

	std::vector< Token * > tokens = lexer.getTokens();

	EXPECT_EQ(tokens.size(), 9);
	EXPECT_EQ(tokens[0]->getValue(), "server");
	EXPECT_EQ(tokens[0]->getTerminal(), T_Server);
	EXPECT_EQ(tokens[1]->getValue(), "{");
	EXPECT_EQ(tokens[1]->getTerminal(), T_OBracket);
	EXPECT_EQ(tokens[2]->getValue(), "return");
	EXPECT_EQ(tokens[2]->getTerminal(), T_Return);
	EXPECT_EQ(tokens[3]->getValue(), "=");
	EXPECT_EQ(tokens[3]->getTerminal(), T_Equal);
	EXPECT_EQ(tokens[4]->getValue(), "200");
	EXPECT_EQ(tokens[4]->getTerminal(), T_ErrorCode);
	EXPECT_EQ(tokens[5]->getValue(), ":");
	EXPECT_EQ(tokens[5]->getTerminal(), T_Colone);
	EXPECT_EQ(tokens[6]->getValue(), "Hello World");
	EXPECT_EQ(tokens[6]->getTerminal(), T_Value);
	EXPECT_EQ(tokens[7]->getValue(), ";");
	EXPECT_EQ(tokens[7]->getTerminal(), T_Semi_Colon);
	EXPECT_EQ(tokens[8]->getValue(), "}");
	EXPECT_EQ(tokens[8]->getTerminal(), T_CBracket);
}

TEST(Tokenize, TokenizeDQuote)
{
	Lexer lexer("../../tests/tokenize/testDQuote.conf");

	lexer.Tokenize();

	std::vector< Token * > tokens = lexer.getTokens();

	EXPECT_EQ(tokens.size(), 9);
	EXPECT_EQ(tokens[0]->getValue(), "server");
	EXPECT_EQ(tokens[0]->getTerminal(), T_Server);
	EXPECT_EQ(tokens[1]->getValue(), "{");
	EXPECT_EQ(tokens[1]->getTerminal(), T_OBracket);
	EXPECT_EQ(tokens[2]->getValue(), "return");
	EXPECT_EQ(tokens[2]->getTerminal(), T_Return);
	EXPECT_EQ(tokens[3]->getValue(), "=");
	EXPECT_EQ(tokens[3]->getTerminal(), T_Equal);
	EXPECT_EQ(tokens[4]->getValue(), "200");
	EXPECT_EQ(tokens[4]->getTerminal(), T_ErrorCode);
	EXPECT_EQ(tokens[5]->getValue(), ":");
	EXPECT_EQ(tokens[5]->getTerminal(), T_Colone);
	EXPECT_EQ(tokens[6]->getValue(), "Hello World");
	EXPECT_EQ(tokens[6]->getTerminal(), T_Value);
	EXPECT_EQ(tokens[7]->getValue(), ";");
	EXPECT_EQ(tokens[7]->getTerminal(), T_Semi_Colon);
	EXPECT_EQ(tokens[8]->getValue(), "}");
	EXPECT_EQ(tokens[8]->getTerminal(), T_CBracket);
}

TEST(Tokenize, TestNoPerm)
{
	EXPECT_THROW(Lexer lexer("../../tests/tokenize/testNoPerm.conf"),
				 Lexer::FileNotOpenException);
}

TEST(Tokenize, TokenizeNotDotConf)
{
	EXPECT_THROW(Lexer lexer("../../tests/tokenize/testNotDotConf"),
				 Lexer::InvalidFileExtensionException);
}

TEST(Tokenize, TestFolderConf)
{
	EXPECT_THROW(Lexer lexer("../../tests/tokenize/Test.conf"),
				 Lexer::FileNotOpenException);
}

TEST(Tokenize, TestSubLink)
{
	EXPECT_THROW(Lexer lexer("../../tests/tokenize/TestLink.conf"),
				 Lexer::FileNotOpenException);
}

TEST(Tokenize, TestNotExisting)
{
	EXPECT_THROW(Lexer lexer("../../tests/tokenize/Truc.conf"),
				 Lexer::FileNotOpenException);
}

int main(int ac, char **av)
{
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}