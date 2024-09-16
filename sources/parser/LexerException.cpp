/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LexerException.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:21:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/16 15:26:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>

#include "parser/Lexer.hpp"

// EXCEPTIONS InvalidFileExtensionException

Lexer::InvalidFileExtensionException::InvalidFileExtensionException(
	const std::string &extention) :
	_msg("Invalid file extension, Found: " + extention + " Expected: .conf")
{
}

Lexer::InvalidFileExtensionException::InvalidFileExtensionException(
	const Lexer::InvalidFileExtensionException &src) :
	_msg(src._msg)
{
}

Lexer::InvalidFileExtensionException &
Lexer::InvalidFileExtensionException::operator=(
	const Lexer::InvalidFileExtensionException &src)
{
	if (this != &src) {
		_msg = src._msg;
	}
	return *this;
}

const char *Lexer::InvalidFileExtensionException::what() const throw()
{
	return _msg.c_str();
}

Lexer::InvalidFileExtensionException::~InvalidFileExtensionException() throw()
{
}

// EXCEPTIONS FileNotOpenException

Lexer::FileNotOpenException::FileNotOpenException(
	const std::string &file_path) :
	_msg("Failed to open file: " + file_path + ": " + std::strerror(errno))
{
}

Lexer::FileNotOpenException::FileNotOpenException(
	const Lexer::FileNotOpenException &src) :
	_msg(src._msg)
{
}

Lexer::FileNotOpenException &
Lexer::FileNotOpenException::operator=(const Lexer::FileNotOpenException &src)
{
	if (this != &src) {
		_msg = src._msg;
	}
	return *this;
}

const char *Lexer::FileNotOpenException::what() const throw()
{
	return _msg.c_str();
}

Lexer::FileNotOpenException::~FileNotOpenException() throw() {}