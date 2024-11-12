/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IdentidyToken.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:16:22 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/12 15:03:33 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest/gtest.h"

#include "lexer/Lexer.hpp"

#define TEST_FILE "conf/identify_token.conf"

TEST(IdentidyToken, IdentidyToken)
{
	Lexer lexer(TEST_FILE);
	lexer.Tokenize();

	std::queue< Token * > tokens = lexer.getTokens();
	Terminal_Type		  terminal[35]{T_Server,
									   T_Location,
									   T_ErrorPage,
									   T_DenyMethod,
									   T_Return,
									   T_Listen,
									   T_Hostname,
									   T_Index,
									   T_AutoIndex,
									   T_Root,
									   T_Log,
									   T_BodySize,
									   T_CGI,
									   T_UploadDir,
									   T_Comma,
									   T_Colon,
									   T_Semi_Colon,
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

	int i = 0;

	while (!tokens.empty()) {
		Token *token = tokens.front();
		EXPECT_EQ(token->getTerminal(), terminal[i]);
		tokens.pop();
		++i;
	}
	EXPECT_EQ(i, 35);
	EXPECT_EQ(tokens.size(), (size_t)0);
}
