/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TokenTest.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 10:21:07 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/15 10:29:12 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "lexer/Token.hpp"

class TokenTest : public ::testing::TestWithParam<std::tuple<Terminal_Type, bool>>
{	
};

TEST_P(TokenTest, IsKey)
{
	Terminal_Type terminal = std::get<0>(GetParam());
	Token token(terminal);

	EXPECT_EQ(Token::IsKey(token), std::get<1>(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(
	IsKey,
	TokenTest,
	::testing::Values(
		std::make_tuple(T_Server, true),
		std::make_tuple(T_Location, true),
		std::make_tuple(T_ErrorPage, true),
		std::make_tuple(T_DenyMethod, true),
		std::make_tuple(T_Return, true),
		std::make_tuple(T_Listen, true),
		std::make_tuple(T_Hostname, true),
		std::make_tuple(T_Index, true),
		std::make_tuple(T_AutoIndex, true),
		std::make_tuple(T_Root, true),
		std::make_tuple(T_Log, true),
		std::make_tuple(T_BodySize, true),
		std::make_tuple(T_CGI, true),
		std::make_tuple(T_UploadDir, true),
		std::make_tuple(T_Bool, false),
		std::make_tuple(T_Digits, false),
		std::make_tuple(T_Method, false),
		std::make_tuple(T_Path, false),
		std::make_tuple(T_Identifier, false),
		std::make_tuple(T_Comma, false),
		std::make_tuple(T_Semi_Colon, false),
		std::make_tuple(T_Colon, false),
		std::make_tuple(T_OBracket, false),
		std::make_tuple(T_CBracket, false),
		std::make_tuple(T_OSquareBracket, false),
		std::make_tuple(T_CSquareBracket, false)
	)
);