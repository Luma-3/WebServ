/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:00:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/16 15:09:15 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Token.hpp"

Token::Token() : _type(None) {}

Token::Token(const Token &src) : _type(src._type), _value(src._value) {}

Token &Token::operator=(const Token &src)
{
	if (this != &src) {
		this->_type = src._type;
		this->_value = src._value;
	}
	return (*this);
}

Token::Token(const std::string &value, enum Token_Type type) :
	_type(type),
	_value(value)
{
}

enum Token_Type Token::getType() const
{
	return (_type);
}

const std::string &Token::getValue() const
{
	return (_value);
}

Token::~Token() {}