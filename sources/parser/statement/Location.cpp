/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:10:50 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/11 11:23:18 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Location.hpp"

#include "lexer/Token.hpp"
#include "parser/statement/DenyMethod.hpp"
#include "parser/statement/Param.hpp"
#include "parser/statement/ParamDouble.hpp"
#include "template/vector_deep_copy.tpp"

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
	_deny_methods(vector_deep_copy(src._deny_methods)),
	_error_pages(vector_deep_copy(src._error_pages))
{
}

Location::Location(std::vector< Token * > &tokens, const std::string &route) :
	Token(S_Location),
	_route(route),
	_autoindex(false)
{
	for (size_t i = 0; i < tokens.size(); ++i) {

		switch (tokens[i]->getType()) {
			case S_Parameter:
				IdentifyParam(tokens[i]);
				break;
			case S_ErrorPage: {
				ErrorPage *error_page = D_Cast< ErrorPage >(tokens[i]);
				_error_pages.push_back(error_page);
				break;
			}
			case S_DenyMethod: {
				DenyMethod *deny_method = D_Cast< DenyMethod >(tokens[i]);
				_deny_methods = deny_method->getMethods();
				delete deny_method;
				break;
			}
			case S_Return: {
				ParamDouble *return_param = D_Cast< ParamDouble >(tokens[i]);
				_return = *return_param;
				delete return_param;
				break;
			}
			default:
				throw Token::InvalidTokenException();
				break;
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
		default:
			break;
	}
	delete param;
}

// bool Location::operator==(const Location &rhs) const
// {
// 	if (this == &rhs) {
// 		return true;
// 	}
// 	if (_route != rhs._route) {
// 		return false;
// 	}
// 	if (_root != rhs._root) {
// 		return false;
// 	}
// 	if (_index != rhs._index) {
// 		return false;
// 	}
// 	if (_autoindex != rhs._autoindex) {
// 		return false;
// 	}
// 	for (size_t i = 0; i < _deny_methods.size(); ++i) {
// 		if (_deny_methods[i] != rhs._deny_methods[i]) {
// 			return false;
// 		}
// 	}
// 	for (size_t i = 0; i < _error_pages.size(); ++i) {
// 		if (_error_pages[i] != rhs._error_pages[i]) {
// 			return false;
// 		}
// 	}
// 	return true;
// }

// bool Location::operator!=(const Location &rhs) const
// {
// 	return !(*this == rhs);
// }

Location::~Location()
{
	std::vector< const statement::ErrorPage * >::const_iterator it =
		_error_pages.begin();

	while (it != _error_pages.end()) {
		delete *it;
		++it;
	}
}

std::ostream &operator<<(std::ostream &o, const statement::Location &location)
{
	o << "Location: " << location.getRoute() << std::endl;
	o << "Root: " << location.getRoot() << std::endl;
	o << "Index: " << location.getIndex() << std::endl;
	o << "Autoindex: " << location.getAutoindex() << std::endl;
	o << "Return: " << location.getReturns() << std::endl;
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