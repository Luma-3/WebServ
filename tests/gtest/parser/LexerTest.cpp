/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LexerTest.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:05:29 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/11 17:13:54 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "gtest/gtest.h"

#include "lexer/Lexer.hpp"

template <typename T>
class LexerTest : public ::testing::Test {
	public:
	void expect_throw_type(const std::string &file_path) {
		EXPECT_THROW(Lexer(file_path.c_str()), T);
	}
};


typedef ::testing::Types<Lexer::InvalidFileExtensionException, 
						  Lexer::FileNotOpenException> MyTypes;

TYPED_TEST_SUITE(LexerTest, MyTypes);

TYPED_TEST(LexerTest, InvalidFileExtensionException) {
	this->expect_throw_type("tests/configs/invalid_extension.txt");
}

TYPED_TEST(LexerTest, FileNotOpenException) {
	this->expect_throw_type("tests/configs/does_not_exist.conf");
}


// int main(int argc, char **argv) {
// 	::testing::InitGoogleTest(&argc, argv);
// 	return RUN_ALL_TESTS();
// }