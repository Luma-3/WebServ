/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParamDouble.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 09:35:39 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/07 18:53:20 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/ParamDouble.hpp"

#include "lexer/Token.hpp"

using statement::ParamDouble;

ParamDouble::ParamDouble() : Token(S_Return) {}

// faire une copy profonde

ParamDouble::ParamDouble(const ParamDouble &src) :
	Token(src),
	_value1(src._value1),
	_value2(src._value2)
{
}

ParamDouble::ParamDouble(const std::string &value1, const std::string &value2,
						 Token_Type type) :
	Token(type),
	_value1(value1),
	_value2(value2)
{
}

const std::string &ParamDouble::getValue1() const
{
	return _value1;
}

const std::string &ParamDouble::getValue2() const
{
	return _value2;
}

ParamDouble &ParamDouble::operator=(const ParamDouble &src)
{
	if (this != &src) {
		Token::operator=(src);
		_value1 = src._value1;
		_value2 = src._value2;
	}
	return *this;
}

ParamDouble::~ParamDouble() {}