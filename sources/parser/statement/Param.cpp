/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:44:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/23 14:08:14 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Param.hpp"

#include "lexer/Token.hpp"

using statement::Param;
using std::string;

Param::Param() : Token(S_Parameter, T_None) {}

Param::Param(const Param &src) : Token(src), _value(src._value) {}

Param::Param(const string &value, enum Terminal_Type key) :
	Token(S_Parameter, key),
	_value(value)
{

	std::cout << "value: " << value << std::endl;
	std::cout << "key: " << key << std::endl;
}

Param &Param::operator=(const Param &src)
{
	if (this != &src) {
		Token::operator=(src);
	}
	return *this;
}

bool Param::ConvertBool(const string &key)
{

	std::cout << "convert key: " << key << std::endl;
	if (key == "on") {
		return true;
	}
	if (key == "off") {
		return false;
	}
	return false;
}

const string &Param::getValue() const
{
	return _value;
}

Param::~Param() {}