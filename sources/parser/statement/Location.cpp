/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:10:50 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 12:21:03 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Location.hpp"

using statement::Location;

Location::Location() : Token(Token_Type::T_Location) {}

Location::Location(const Location &src) :
	Token(src),
	_root(src._root),
	_index(src._index),
	_autoindex(src._autoindex),
	_return(src._return),
	_deny_methods(src._deny_methods),
	_error_pages(src._error_pages)
{
}

Location &Location::operator=(const Location &src)
{
	if (this != &src) {
		Token::operator=(src);
		_root = src._root;
		_index = src._index;
		_autoindex = src._autoindex;
		_return = src._return;
		_deny_methods = src._deny_methods;
		_error_pages = src._error_pages;
	}
	return *this;
}

Location::~Location() {}