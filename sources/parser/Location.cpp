/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:10:50 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/13 12:39:21 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Location.hpp"

#include "lexer/Token.hpp"
#include "template/vector_deep_copy.tpp"

Location::Location(const std::string &route) : _route(route) {}

// faire une copy profonde

Location::Location(const Location &src) : _route(src._route) {}

Location &Location::operator=(const Location &src)
{
	if (this != &src) {
		_route = src._route;
	}
	return *this;
}

bool Location::operator==(const Location &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	if (_route != rhs._route) {
		return false;
	}
	for (std::map< std::string, Param * >::const_iterator it = _params.begin();
		 it != _params.end(); ++it) {
		if (it->second != rhs._params.at(it->first)) {
			return false;
		}
	}
	return true;
}

void Location::addParam(const std::string &key, Param *param)
{
	_params[key] = param;
}

const Param *Location::getParam(const std::string &key) const
{
	return _params.at(key);
}

void Location::print() const
{
	std::cout << "Location : " << _route << std::endl;
	std::map< std::string, Param * >::const_iterator it = _params.begin();

	while (it != _params.end()) {
		std::cout << "Key : " << it->first << " Value : ";
		it->second->print();
		++it;
	}
}

// void Location::IdentifyParam(Token *token)
// {
// 	Param *param = dynamic_cast< Param * >(token);
// 	switch (param->getTerminal()) {
// 		case T_Root:
// 			_root = param->getValue();
// 			break;
// 		case T_Index:
// 			_index = param->getValue();
// 			break;
// 		case T_AutoIndex:
// 			_autoindex = Param::ConvertBool(param->getValue());
// 			break;
// 		default:
// 			break;
// 	}
// 	delete param;
// }

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
	for (std::map< std::string, Param * >::iterator it = _params.begin();
		 it != _params.end(); ++it) {
		delete it->second;
	}
}
