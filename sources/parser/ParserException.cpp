/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:19:55 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 11:52:44 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Parser.hpp"

#

using parser::Parser;

Parser::InvalidTokenException::InvalidTokenException(
	size_t _col, size_t _line, const std::string &value,
	const std::string &expected) :
	_msg("Error: unexpected token: " + value + " at line: " + ToString(_line) +
		 " | col: " + ToString(_col) + " expected: " + expected)
{
}

const char *Parser::InvalidTokenException::what() const throw()
{
	return _msg.c_str();
}

Parser::InvalidTokenException::InvalidTokenException(
	const InvalidTokenException &src) :
	_msg(src._msg)
{
}

Parser::InvalidTokenException &
Parser::InvalidTokenException::operator=(const InvalidTokenException &src)
{
	if (this != &src) {
		_msg = src._msg;
	}
	return *this;
}

Parser::InvalidTokenException::~InvalidTokenException() throw() {}

Parser::MissingParamException::MissingParamException(const std::string &value) :
	_msg("Error: missing parameter: " + value)
{
}

const char *Parser::MissingParamException::what() const throw()
{
	return _msg.c_str();
}

Parser::MissingParamException::MissingParamException(
	const MissingParamException &src) :
	_msg(src._msg)
{
}

Parser::MissingParamException &
Parser::MissingParamException::operator=(const MissingParamException &src)
{
	if (this != &src) {
		_msg = src._msg;
	}
	return *this;
}

Parser::MissingParamException::~MissingParamException() throw() {}