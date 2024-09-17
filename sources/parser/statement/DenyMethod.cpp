/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:17:57 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 11:19:15 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/DenyMethod.hpp"

using statement::DenyMethod;

DenyMethod::DenyMethod() : Token(Token_Type::T_DenyMethod) {}

DenyMethod::DenyMethod(const DenyMethod &src) :
	Token(src),
	_methods(src._methods)
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