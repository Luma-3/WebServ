/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:00:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/13 14:12:07 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Token.hpp"

Token::Token() : _type(None) {}

Token::Token(const Token &src)
{
	*this = src;
}

Token &Token::operator=(const Token &src)
{
	if (this == &src) {
		*this = src;
	}
	return (*this);
}

Token::Token(std::string value, enum Token_Type type) :
	_type(type),
	_value(value)
{
}

Token::~Token() {}