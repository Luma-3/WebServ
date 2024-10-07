/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:44:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/07 16:40:25 by jbrousse         ###   ########.fr       */
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
	}
	if (key == "off") {
		return false;
	}
	return false;
}

string Param::TrimHostName(const string &host)
{
	string new_host = host.substr(1, host.size() - 1);
	return new_host;
}

const string &Param::getValue() const
{
	return _value;
}

Param::~Param() {}