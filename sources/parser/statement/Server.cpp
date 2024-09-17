/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:05:48 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 13:07:23 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/statement/Server.hpp"

using statement::Server;

Server::Server() : Token(Token_Type::T_ServerBlock), _autoindex(false) {}

Server::Server(const Server &src) :
	Token(src),
	_port(src._port),
	_server_name(src._server_name),
	_root(src._root),
	_index(src._index),
	_autoindex(src._autoindex),
	_return(src._return),
	_deny_methods(src._deny_methods),
	_error_pages(src._error_pages),
	_locations(src._locations)
{
}

Server &Server::operator=(const Server &src)
{
	if (this != &src) {
		Token::operator=(src);
		_port = src._port;
		_server_name = src._server_name;
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

Server::~Server() {}