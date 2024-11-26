/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IdentidyToken.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:16:22 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/22 14:42:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest/gtest.h"

#include "lexer/Lexer.hpp"

class IdentidyToken :
	public ::testing::TestWithParam<
		std::tuple< std::string, std::vector< Terminal_Type > > >
{
  protected:
	std::string					 _file_path;
	std::vector< Terminal_Type > _terminals;

  public:
	IdentidyToken() :
		_file_path(std::get< 0 >(GetParam())),
		_terminals(std::get< 1 >(GetParam()))
	{
	}
};

TEST_P(IdentidyToken, Terminal)
{
	Lexer lexer(_file_path.c_str());
	lexer.Tokenize();

	std::queue< Token * > tokens = lexer.getTokens();
	size_t				  i = 0;

	while (!tokens.empty()) {
		Token *token = tokens.front();
		EXPECT_EQ(token->getTerminal(), _terminals[i]);
		tokens.pop();
		++i;
	}
	size_t size = _terminals.size();

	EXPECT_EQ(i, (size_t)size);
	EXPECT_EQ(tokens.size(), (size_t)0);
}

std::vector< Terminal_Type > term = {T_Server,
									 T_Location,
									 T_ErrorPage,
									 T_KeyMethod,
									 T_Return,
									 T_Listen,
									 T_ServerName,
									 T_Index,
									 T_AutoIndex,
									 T_Root,
									 T_Log,
									 T_BodySize,
									 T_CGI,
									 T_Comma,
									 T_Colon,
									 T_SemiColon,
									 T_OBracket,
									 T_CBracket,
									 T_OSquareBracket,
									 T_CSquareBracket,
									 T_Identifier,
									 T_Digits,
									 T_Method,
									 T_Method,
									 T_Method,
									 T_Path,
									 T_CGIExtension,
									 T_CGIExtension,
									 T_Bool,
									 T_Bool,
									 T_LogLevel,
									 T_LogLevel,
									 T_LogLevel,
									 T_LogLevel};

std::vector< Terminal_Type > empty = {};

INSTANTIATE_TEST_SUITE_P(
	Terminal, IdentidyToken,
	::testing::Values(
		std::make_tuple("conf/lexer/identify_token.conf", term),
		std::make_tuple("conf/lexer/identify_token_tab.conf", term),
		std::make_tuple("conf/lexer/identify_token_tab+space.conf", term),
		std::make_tuple("conf/lexer/identify_token_one_line.conf", term),
		std::make_tuple("conf/lexer/empty.conf", empty)));
