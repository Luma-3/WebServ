/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:44:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 16:22:45 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Param.hpp"

using statement::Param;
using std::string;

Param::Param() : Token(S_Parameter, T_None) {}

Param::Param(const Param &src) : Token(src), _value(src._value) {}

Param::Param(const string &value, enum Terminal_Type key) :
	Token(S_Parameter, key),
	_value(value)
{
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
	if (key == "on") {
		return true;
	} else if (key == "off") {
		return false;
	}
	return false; // TODO : throw exception
}

const string &Param::getValue() const
{
	return _value;
}

Param::~Param() {}