/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:05:48 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/24 12:49:23 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Server.hpp"

#include <iostream>

#include "lexer/Token.hpp"
#include "parser/statement/DenyMethod.hpp"
#include "parser/statement/ErrorPage.hpp"
#include "parser/statement/Location.hpp"
#include "parser/statement/Param.hpp"
#include "parser/statement/ReturnParam.hpp"

using statement::Server;

Server::Server() : Token(S_Server), _autoindex(false) {}

Server::Server(const Server &src) :
	Token(src),
	_port(src._port),
	_host(src._host),
	_root(src._root),
	_index(src._index),
	_autoindex(src._autoindex),
	_return(src._return),
	_deny_methods(src._deny_methods),
	_error_pages(src._error_pages),
	_locations(src._locations)
{
}

void Server::IdentifyParam(Token *token)
{
	Param *param = dynamic_cast< Param * >(token);
	switch (param->getTerminal()) {
		case T_Port:
			_port = param->getValue();
			break;
		case T_Host:
			_host = Param::TrimHostName(param->getValue());
			break;
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

Server::Server(const std::vector< Token * > &tokens) : _autoindex(false)
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
		} else if (tokens[i]->Token::getType() == S_Location) {
			statement::Location *location =
				dynamic_cast< statement::Location * >(tokens[i]);
			_locations.push_back(location);
		} else {
			// TODO : throw exception
			std::cerr << tokens[i]->getType() << std::endl;
			std::cerr << tokens[i]->getValue() << std::endl;
			std::cerr << "i :" << i << std::endl;
			std::cerr << "Error: Server::Server(const std::vector< Token * > "
						 "&tokens) - Unknown token type"
					  << std::endl;
		}
	}
}

Server &Server::operator=(const Server &src)
{
	if (this != &src) {
		Token::operator=(src);
		_port = src._port;
		_host = src._host;
		_root = src._root;
		_index = src._index;
		_autoindex = src._autoindex;
		_return = src._return;
		_deny_methods = src._deny_methods;
		_error_pages = src._error_pages;
		_locations = src._locations;
	}
	return *this;
}

bool Server::operator==(const Server &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	if (_port != rhs._port) {
		return false;
	}
	if (_host != rhs._host) {
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
	for (size_t i = 0; i < _locations.size(); ++i) {
		if (*_locations[i] != *rhs._locations[i]) {
			return false;
		}
	}
	return true;
}

Server::~Server() {}

std::ostream &operator<<(std::ostream &o, const statement::Server &server)
{
	o << "Server: " << server.getHost() << std::endl;
	o << "Port: " << server.getPort() << std::endl;
	o << "Root: " << server.getRoot() << std::endl;
	o << "Index: " << server.getIndex() << std::endl;
	o << "Autoindex: " << server.getAutoindex() << std::endl;
	o << "Return: " << server.getReturn() << std::endl;
	std::vector< std::string > deny_methods = server.getDenyMethods();
	for (size_t i = 0; i < deny_methods.size(); ++i) {
		o << "Deny method: " << deny_methods[i] << std::endl;
	}
	std::vector< statement::ErrorPage > error_pages = server.getErrorPages();
	for (size_t i = 0; i < error_pages.size(); ++i) {
		o << "Error page: " << error_pages[i] << std::endl;
	}
	std::vector< const statement::Location * > locations =
		server.getLocations();
	for (size_t i = 0; i < locations.size(); ++i) {
		o << "Location: " << *locations[i] << std::endl;
	}
	return o;
}