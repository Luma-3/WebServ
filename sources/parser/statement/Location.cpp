/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:10:50 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/01 12:29:41 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Location.hpp"

#include "lexer/Token.hpp"
#include "parser/statement/DenyMethod.hpp"
#include "parser/statement/Param.hpp"
#include "parser/statement/ReturnParam.hpp"

using statement::Location;

Location::Location() : Token(S_Location), _autoindex(false) {}

// faire une copy profonde

Location::Location(const Location &src) :
	Token(src),
	_route(src._route),
	_root(src._root),
	_index(src._index),
	_autoindex(src._autoindex),
	_return(src._return),
	_deny_methods(src._deny_methods),
	_error_pages(src._error_pages)
{
}

Location::Location(const std::vector< Token * > &tokens,
				   const std::string			&route) :
	Token(S_Location),
	_route(route),
	_autoindex(false)
{

	std::cout << "Location constructor" << std::endl;
	std::cout << "Route : " << route << std::endl;
	for (size_t i = 0; i < tokens.size(); ++i) {
		if (tokens[i]->Token::getType() == S_Parameter) {
			IdentifyParam(tokens[i]);
		} else if (tokens[i]->Token::getType() == S_ErrorPage) {
			statement::ErrorPage *error_page =
				dynamic_cast< statement::ErrorPage * >(tokens[i]);
			_error_pages.push_back(error_page);
		} else if (tokens[i]->Token::getType() == S_DenyMethod) {
			_deny_methods =
				dynamic_cast< DenyMethod * >(tokens[i])->getMethods();
		} else if (tokens[i]->Token::getType() == S_Return) {
			_return = dynamic_cast< ReturnParam * >(tokens[i])->getValue();
		} else {
			std::cerr
				<< "Error: Location::Location(const std::vector< Token * > "
				   "&tokens) - Unknown token type"
				<< std::endl;

			// TODO : throw exception
		}
	}
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

void Location::IdentifyParam(Token *token)
{
	Param *param = dynamic_cast< Param * >(token);
	switch (param->getTerminal()) {
		case T_Root:
			_root = param->getValue();
			break;
		case T_Index:
			_index = param->getValue();
			break;
		case T_AutoIndex:
			_autoindex = Param::ConvertBool(param->getValue());
			break;
		case T_Return:
			_return = param->getValue();
			break;
		default:
			break;
	}
}

bool Location::operator==(const Location &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	if (_route != rhs._route) {
		return false;
	}
	if (_root != rhs._root) {
		return false;
	}
	if (_index != rhs._index) {
		return false;
	}
	if (_autoindex != rhs._autoindex) {
		return false;
	}
	if (_return != rhs._return) {
		return false;
	}
	for (size_t i = 0; i < _deny_methods.size(); ++i) {
		if (_deny_methods[i] != rhs._deny_methods[i]) {
			return false;
		}
	}
	for (size_t i = 0; i < _error_pages.size(); ++i) {
		if (_error_pages[i] != rhs._error_pages[i]) {
			return false;
		}
	}
	return true;
}

bool Location::operator!=(const Location &rhs) const
{
	return !(*this == rhs);
}

Location::~Location() {}

std::ostream &operator<<(std::ostream &o, const statement::Location &location)
{
	o << "Location: " << location.getRoute() << std::endl;
	o << "Root: " << location.getRoot() << std::endl;
	o << "Index: " << location.getIndex() << std::endl;
	o << "Autoindex: " << location.getAutoindex() << std::endl;
	o << "Return: " << location.getReturn() << std::endl;
	std::vector< std::string > deny_methods = location.getDenyMethods();
	for (size_t i = 0; i < deny_methods.size(); ++i) {
		o << "Deny method: " << deny_methods[i] << std::endl;
	}
	std::vector< const statement::ErrorPage * > error_pages =
		location.getErrorPages();
	for (size_t i = 0; i < error_pages.size(); ++i) {
		o << error_pages[i];
	}
	return o;
}