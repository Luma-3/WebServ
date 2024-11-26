/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegexTest.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 10:31:09 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/15 14:37:06 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "lexer/Token.hpp"

#define NONE			 0b00000000000
#define IS_BOOL			 0b00000000001
#define IS_METHOD		 0b00000000010
#define IS_LOG_LEVEL	 0b00000000100
#define IS_PATH			 0b00000001000
#define IS_DIGIT		 0b00000010000
#define IS_ERROR_CODE	 0b00000100000
#define IS_HOSTNAME		 0b00001000000
#define IS_BODY_SIZE	 0b00010000000
#define IS_IP			 0b00100000000
#define IS_PORT			 0b01000000000
#define IS_CGI_EXTENSION 0b10000000000

class regexFunctionTest :
	public ::testing::TestWithParam< std::tuple< std::string, int > >
{
};

TEST_P(regexFunctionTest, IsBool)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsBool(value), (bool)(expected & IS_BOOL));
}

TEST_P(regexFunctionTest, IsMethod)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsMethod(value), (bool)(expected & IS_METHOD));
}

TEST_P(regexFunctionTest, IsLogLevel)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsLogLevel(value), (bool)(expected & IS_LOG_LEVEL));
}

TEST_P(regexFunctionTest, IsPath)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsPath(value), (bool)(expected & IS_PATH));
}

TEST_P(regexFunctionTest, IsDigit)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsDigit(value), (bool)(expected & IS_DIGIT));
}

TEST_P(regexFunctionTest, IsErrorCode)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsErrorCode(value), (bool)(expected & IS_ERROR_CODE));
}

TEST_P(regexFunctionTest, IsHostname)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsHostname(value), (bool)(expected & IS_HOSTNAME));
}

TEST_P(regexFunctionTest, IsBodySize)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsBodySize(value), (bool)(expected & IS_BODY_SIZE));
}

TEST_P(regexFunctionTest, IsIP)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsIP(value), (bool)(expected & IS_IP));
}

TEST_P(regexFunctionTest, IsPort)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsPort(value), (bool)(expected & IS_PORT));
}

TEST_P(regexFunctionTest, IsCGIExtension)
{
	std::string value = std::get< 0 >(GetParam());
	int			expected = std::get< 1 >(GetParam());

	EXPECT_EQ(IsCGIExtension(value), (bool)(expected & IS_CGI_EXTENSION));
}

// 56
INSTANTIATE_TEST_SUITE_P(
	RegexTest, regexFunctionTest,
	::testing::Values(
		std::make_tuple("on", IS_BOOL | IS_HOSTNAME),
		std::make_tuple("off", IS_BOOL | IS_HOSTNAME),
		std::make_tuple("yes", NONE | IS_HOSTNAME),
		std::make_tuple("no", NONE | IS_HOSTNAME),
		std::make_tuple("true", NONE | IS_HOSTNAME),
		std::make_tuple("false", NONE | IS_HOSTNAME),
		std::make_tuple("1", IS_PORT | IS_DIGIT | IS_BODY_SIZE | IS_HOSTNAME),
		std::make_tuple("0", IS_PORT | IS_DIGIT | IS_BODY_SIZE | IS_HOSTNAME),
		std::make_tuple("0.0.0.1", IS_IP | IS_HOSTNAME),
		std::make_tuple("505", IS_ERROR_CODE | IS_DIGIT | IS_HOSTNAME |
								   IS_BODY_SIZE | IS_PORT),
		std::make_tuple("exemple.com", IS_HOSTNAME),
		std::make_tuple("8080",
						IS_PORT | IS_DIGIT | IS_BODY_SIZE | IS_HOSTNAME),
		std::make_tuple("php", IS_CGI_EXTENSION | IS_HOSTNAME),
		std::make_tuple("py", IS_CGI_EXTENSION | IS_HOSTNAME),
		std::make_tuple("js", NONE | IS_HOSTNAME),
		std::make_tuple("test/", IS_PATH),
		std::make_tuple("test/test/", IS_PATH),
		std::make_tuple("test/2/test/", IS_PATH),
		std::make_tuple("GET", IS_METHOD | IS_HOSTNAME),
		std::make_tuple("POST", IS_METHOD | IS_HOSTNAME),
		std::make_tuple("DELETE", IS_METHOD | IS_HOSTNAME),
		std::make_tuple("gET", IS_METHOD | IS_HOSTNAME),
		std::make_tuple("pOst", IS_METHOD | IS_HOSTNAME),
		std::make_tuple("deLetE", IS_METHOD | IS_HOSTNAME),
		std::make_tuple("debug", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("info", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("warning", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("error", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("DEBUG", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("INFO", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("WARNING", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("ERROR", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("ErRor", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("WaRnInG", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("InFo", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("DeBuG", IS_LOG_LEVEL | IS_HOSTNAME),
		std::make_tuple("1K", IS_BODY_SIZE | IS_HOSTNAME),
		std::make_tuple("1M", IS_BODY_SIZE | IS_HOSTNAME),
		std::make_tuple("1G", IS_BODY_SIZE | IS_HOSTNAME),
		std::make_tuple("1k", IS_BODY_SIZE | IS_HOSTNAME),
		std::make_tuple("1m", IS_BODY_SIZE | IS_HOSTNAME),
		std::make_tuple("1g", IS_BODY_SIZE | IS_HOSTNAME),

		std::make_tuple("-1", IS_HOSTNAME),
		std::make_tuple("127.0.1.3.4", IS_HOSTNAME),
		std::make_tuple("onn", IS_HOSTNAME),
		std::make_tuple("/je/suis/un/path/awdad", NONE),
		std::make_tuple("fichier.txt", IS_HOSTNAME),
		std::make_tuple("POSTT", IS_HOSTNAME),
		std::make_tuple("PUT", IS_HOSTNAME),
		std::make_tuple("fichier.python", IS_HOSTNAME),
		std::make_tuple("DEBUGING", IS_HOSTNAME),
		std::make_tuple("WARING", IS_HOSTNAME),
		std::make_tuple("EROR", IS_HOSTNAME),
		std::make_tuple("INFOX", IS_HOSTNAME),
		std::make_tuple(
			"20349820349820394823049283049283409823409823409283409KB",
			IS_HOSTNAME)));