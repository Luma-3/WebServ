/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TokenTest.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 10:21:07 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/22 14:41:24 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Token.hpp"

#include <gtest/gtest.h>

class TokenTest :
	public ::testing::TestWithParam< std::tuple< Terminal_Type, bool > >
{
};

TEST_P(TokenTest, IsKey)
{
	Terminal_Type terminal = std::get< 0 >(GetParam());
	Token		  token(terminal);

	EXPECT_EQ(Token::IsKey(token), std::get< 1 >(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(
	IsKey, TokenTest,
	::testing::Values(
		std::make_tuple(T_Server, true), std::make_tuple(T_Location, true),
		std::make_tuple(T_ErrorPage, true), std::make_tuple(T_KeyMethod, true),
		std::make_tuple(T_Return, true), std::make_tuple(T_Listen, true),
		std::make_tuple(T_ServerName, true), std::make_tuple(T_Index, true),
		std::make_tuple(T_AutoIndex, true), std::make_tuple(T_Root, true),
		std::make_tuple(T_Log, true), std::make_tuple(T_BodySize, true),
		std::make_tuple(T_CGI, true), std::make_tuple(T_UploadDir, true),
		std::make_tuple(T_Bool, false), std::make_tuple(T_Digits, false),
		std::make_tuple(T_Method, false), std::make_tuple(T_Path, false),
		std::make_tuple(T_Identifier, false), std::make_tuple(T_Comma, false),
		std::make_tuple(T_SemiColon, false), std::make_tuple(T_Colon, false),
		std::make_tuple(T_OBracket, false), std::make_tuple(T_CBracket, false),
		std::make_tuple(T_OSquareBracket, false),
		std::make_tuple(T_CSquareBracket, false)));