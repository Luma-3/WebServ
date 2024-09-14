/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestTokenize.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:49:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/14 16:42:13 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

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

TEST(Tokenize, skipSpace)
{
	std::string line = "    server";
	size_t		it = 0;

	it = skip_space(line,
					it); // TODO pass static function on static method class

	EXPECT_EQ(it, 4);
}

int main(int ac, char **av)
{
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}