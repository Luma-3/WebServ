/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:44:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 13:03:15 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Param.hpp"

using statement::Param;

Param::Param() : Token(Token_Type::T_Parameter), _param_type(Param_Type::None)
{
}

Param::Param(const Param &src) :
	Token(src),
	_param_type(src._param_type),
	_error_code(src._error_code),
	_value(src._value)
{
}

Param &Param::operator=(const Param &src)
{
	if (this != &src) {
		Token::operator=(src);
		_param_type = src._param_type;
		_error_code = src._error_code;
		_value = src._value;
	}
	return *this;
}

Param::~Param() {}