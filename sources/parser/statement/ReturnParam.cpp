/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnParam.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 09:35:39 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/27 10:15:57 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/ReturnParam.hpp"

#include "lexer/Token.hpp"

using statement::ReturnParam;

ReturnParam::ReturnParam() : Token(S_Return) {}

// faire une copy profonde

ReturnParam::ReturnParam(const ReturnParam &src) :
	Token(src),
	_error_code(src._error_code),
	_value(src._value)
{
}

ReturnParam::ReturnParam(const std::string &error_code,
						 const std::string &value) :
	Token(S_Return),
	_error_code(error_code),
	_value(value)
{
}

ReturnParam &ReturnParam::operator=(const ReturnParam &src)
{
	if (this != &src) {
		Token::operator=(src);
	}
	return *this;
}

ReturnParam::~ReturnParam() {}