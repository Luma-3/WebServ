/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestTokenize.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:49:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 20:02:00 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "Parser.hpp"

TEST(Tokenize, test1)
{
	std::ifstream			 file("../../tests/tokenize/test1.conf");
	std::vector<std::string> tokens = Tokenize(file);

	EXPECT_EQ(tokens.size(), 7);
	EXPECT_EQ(tokens[0], "server");
	EXPECT_EQ(tokens[1], "{");
	EXPECT_EQ(tokens[2], "listen");
	EXPECT_EQ(tokens[3], "=");
	EXPECT_EQ(tokens[4], "8080");
	EXPECT_EQ(tokens[5], ";");
	EXPECT_EQ(tokens[6], "}");
}

TEST(Tokenize, test2)
{
	std::ifstream			 file("../../tests/tokenize/test2.conf");
	std::vector<std::string> tokens = Tokenize(file);

	EXPECT_EQ(tokens.size(), 7);
	EXPECT_EQ(tokens[0], "server");
	EXPECT_EQ(tokens[1], "{");
	EXPECT_EQ(tokens[2], "listen");
	EXPECT_EQ(tokens[3], "=");
	EXPECT_EQ(tokens[4], "8080");
	EXPECT_EQ(tokens[5], ";");
	EXPECT_EQ(tokens[6], "}");
}

TEST(Tokenize, test3)
{
	std::ifstream			 file("../../tests/tokenize/test3.conf");
	std::vector<std::string> tokens = Tokenize(file);

	EXPECT_EQ(tokens.size(), 14);
	EXPECT_EQ(tokens[0], "server");
	EXPECT_EQ(tokens[1], "{");
	EXPECT_EQ(tokens[2], "listen");
	EXPECT_EQ(tokens[3], "=");
	EXPECT_EQ(tokens[4], "8080");
	EXPECT_EQ(tokens[5], ";");
	EXPECT_EQ(tokens[6], "location");
	EXPECT_EQ(tokens[7], "{");
	EXPECT_EQ(tokens[8], "root");
	EXPECT_EQ(tokens[9], "=");
	EXPECT_EQ(tokens[10], "/var/www/html");
	EXPECT_EQ(tokens[11], ";");
	EXPECT_EQ(tokens[12], "}");
	EXPECT_EQ(tokens[13], "}");
}

int main(int ac, char **av)
{
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}