/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:17:57 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/02 12:32:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/DenyMethod.hpp"

#include "lexer/Token.hpp"
#include "template/vector_deep_copy.tpp"

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
	_methods(vector_deep_copy(methods))
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

DenyMethod::~DenyMethod()
{
}

const std::vector< std::string > &DenyMethod::getMethods() const
{
	return _methods;
}

std::ostream &statement::operator<<(std::ostream	 &o,
									const DenyMethod &denyMethod)
{
	o << "DenyMethod: ";
	std::vector< std::string > methods = denyMethod.getMethods();
	for (size_t i = 0; i < methods.size(); ++i) {
		o << methods[i] << " ";
	}
	return o;
}