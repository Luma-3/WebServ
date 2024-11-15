/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReduceTest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:19 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/15 12:41:16 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "parser/Parser.hpp"

using parser::Parser;

TEST(ReduceTest, R1_Server)
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

	EXPECT_EQ(parser.getServers().size(), (size_t)1);
}