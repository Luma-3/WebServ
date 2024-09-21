/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:10:50 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/21 14:35:50 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Location.hpp"

#include "lexer/Token.hpp"
#include "parser/statement/DenyMethod.hpp"
#include "parser/statement/Param.hpp"

using statement::DenyMethod;
using statement::Location;
using statement::Param;

Location::Location() : Token(S_Location), _autoindex(false) {}

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

Location::Location(const std::vector< Token * > &tokens,
				   const std::string			&route) :
	Token(S_Location),
	_route(route),
	_autoindex(false)
{
	for (size_t i = 0; i < tokens.size(); ++i) {
		if (tokens[i]->Token::getType() == S_Parameter) {
			IdentifyParam(tokens[i]);
		} else if (tokens[i]->Token::getType() == S_ErrorPage) {
			statement::ErrorPage *error_page =
				dynamic_cast< statement::ErrorPage * >(tokens[i]);
			_error_pages.push_back(*error_page);
		} else if (tokens[i]->Token::getType() == S_DenyMethod) {
			_deny_methods =
				dynamic_cast< DenyMethod * >(tokens[i])->getMethods();
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
	std::vector< statement::ErrorPage > error_pages = location.getErrorPages();
	for (size_t i = 0; i < error_pages.size(); ++i) {
		o << error_pages[i];
	}
	return o;
}