/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:20:36 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 11:24:39 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/ErrorPage.hpp"

using statement::ErrorPage;

ErrorPage::ErrorPage() : Token(Token_Type::T_ErrorPage) {}

ErrorPage::ErrorPage(const ErrorPage &src) :
	Token(src),
	_error_code(src._error_code),
	_value(src._value)
{
}

ErrorPage &ErrorPage::operator=(const ErrorPage &src)
{
	if (this != &src) {
		Token::operator=(src);
		_error_code = src._error_code;
		_value = src._value;
	}
	return *this;
}

ErrorPage::~ErrorPage() {}