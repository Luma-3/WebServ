/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:17:57 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/27 10:16:15 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/DenyMethod.hpp"

#include "lexer/Token.hpp"

using statement::DenyMethod;

DenyMethod::DenyMethod() : Token(S_DenyMethod) {}

// faire une copy profonde

DenyMethod::DenyMethod(const DenyMethod &src) :
	Token(src),
	_methods(src._methods)
{
}

DenyMethod::DenyMethod(const std::vector< std::string > &methods) :
	Token(S_DenyMethod),
	_methods(methods)
{
}

DenyMethod &DenyMethod::operator=(const DenyMethod &src)
{
	if (this != &src) {
		Token::operator=(src);
		_methods = src._methods;
	}
	return *this;
}

DenyMethod::~DenyMethod() {}

const std::vector< std::string > &DenyMethod::getMethods() const
{
	return _methods;
}