/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LexerOpenConf.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:05:29 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/12 14:15:38 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gtest/gtest.h"
#include <string>

#include "lexer/Lexer.hpp"

TEST(LexerTest, InvalidFileExtensionException)
{

	EXPECT_THROW(Lexer("conf/exception/bad_extension.d"),
				 Lexer::InvalidFileExtensionException);
}

TEST(LexerTest, MissExtensionException)
{
	EXPECT_THROW(Lexer("conf/exception/miss_extension"),
				 Lexer::InvalidFileExtensionException);
}

TEST(LexerTest, FileNotExistException)
{
	EXPECT_THROW(Lexer("conf/exception/does_not_exist.conf"),
				 Lexer::FileNotOpenException);
}

TEST(LexerTest, NoPermissionException)
{
	EXPECT_THROW(Lexer("conf/exception/no_perm.conf"),
				 Lexer::FileNotOpenException);
}

TEST(LexerTest, IsDirectoryException)
{
	EXPECT_THROW(Lexer("conf/exception/dir.conf"), Lexer::FileNotOpenException);
}

TEST(LexerTest, EmptyFileException)
{
	EXPECT_NO_THROW(Lexer("conf/empty.conf"));
}

// class LexerFixture : public ::testing::TestWithParam< std::string >
// {
//   protected:
// 	Lexer *_lexer;

// 	void SetUp() override { _lexer = nullptr; }

// 	void TearDown() override
// 	{
// 		delete _lexer;
// 		_lexer = nullptr;
// 	}

// 	void CreateLexer(const std::string &file_path)
// 	{
// 		_lexer = new Lexer(file_path.c_str());
// 	}
// };
