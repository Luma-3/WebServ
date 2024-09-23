/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:20:36 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/23 15:15:59 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/ErrorPage.hpp"

#include "lexer/Token.hpp"

using statement::ErrorPage;

ErrorPage::ErrorPage() : Token(S_ErrorPage) {}

ErrorPage::ErrorPage(const ErrorPage &src) :
	Token(src),
	_error_code(src._error_code),
	_value(src._value)
{
}

ErrorPage::ErrorPage(const std::vector< std::string > &error_code,
					 const std::string				  &value) :
	Token(S_ErrorPage),
	_error_code(error_code),
	_value(value)
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

bool ErrorPage::operator==(const ErrorPage &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	for (size_t i = 0; i < _error_code.size(); ++i) {
		if (_error_code[i] != rhs._error_code[i]) {
			return false;
		}
	}
	return (_value == rhs._value);
}

bool ErrorPage::operator!=(const ErrorPage &rhs) const
{
	return !(*this == rhs);
}

ErrorPage::~ErrorPage() {}

std::ostream &operator<<(std::ostream				&o,
						 const statement::ErrorPage &error_page)
{
	o << "ErrorPage: " << error_page.getValue() << std::endl;
	std::vector< std::string > error_code = error_page.getErrorCode();
	for (size_t i = 0; i < error_code.size(); ++i) {
		o << "Error code: " << error_code[i] << std::endl;
	}
	return o;
}