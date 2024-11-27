/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:19:55 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/27 11:05:26 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.hpp"
#include "parser/Parser.hpp"

using parser::Parser;

Parser::UnexpectedTokenException::UnexpectedTokenException(
	size_t _col, size_t _line, const std::string &value,
	const std::string &expected) :
	_msg(PASTEL_RED "Error:" ORANGE " unexpected token: \n" RESET "line: " +
		 ToString(_line) + " | col: " + ToString(_col) + " >>> " + value +
		 LIME "\nExpected: " RESET + expected + RESET)
{
}

const char *Parser::UnexpectedTokenException::what() const throw()
{
	return _msg.c_str();
}

Parser::UnexpectedTokenException::UnexpectedTokenException(
	const UnexpectedTokenException &src) :
	_msg(src._msg)
{
}

Parser::UnexpectedTokenException &
Parser::UnexpectedTokenException::operator=(const UnexpectedTokenException &src)
{
	if (this != &src) {
		_msg = src._msg;
	}
	return *this;
}

Parser::UnexpectedTokenException::~UnexpectedTokenException() throw() {}

Parser::MissingParamException::MissingParamException(const std::string &value) :
	_msg(PASTEL_RED "Error:" ORANGE " missing parameter: " RESET "`" + value +
		 "`")
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
