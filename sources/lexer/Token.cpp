/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:00:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 14:46:10 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Token.hpp"

using std::string;

Token::Token() : _type(T_None) {}

Token::Token(const Token &src) : _type(src._type), _value(src._value) {}

Token &Token::operator=(const Token &src)
{
	if (this != &src) {
		this->_type = src._type;
		this->_value = src._value;
	}
	return (*this);
}

Token::Token(enum Token_Type type) : _type(type) {}

Token::Token(const std::string &value, enum Token_Type type) :
	_type(type),
	_value(value)
{
}

Token::~Token() {}

enum Token_Type Token::getType() const
{
	return (_type);
}

const std::string &Token::getValue() const
{
	return (_value);
}

Token_Type Token::IdentifyToken(const string &value)
{
	static const int	size_key = 13;
	static const string key[size_key] = {
		"server", "listen",	  "error_log",	"acces_log",
		"root",	  "index",	  "autoindex",	"host",
		"return", "location", "error_page", "deny_method"};

	for (size_t i = 0; i < size_key; ++i) {
		if (value == key[i]) {
			return (T_Key);
		}
	}

	// TODO: Check if value is a error code

	char c = *value.c_str();
	switch (c) {
		case ',':
			return (T_Comma);
		case ':':
			return (T_Colone);
		case ';':
			return (T_Semi_Colon);
		case '{':
		case '}':
			return (T_Bracket);
		case '=':
			return (T_Equal);
		default:
			break;
	}
	return (T_Value);
}