/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/19 16:20:21 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

#include <exception>
#include <unistd.h>

using client::Client;

Client::Client() : _server(NULL), _default_server(NULL), _client_socket(-1) {}

Client::Client(const VirtualServer *server, const VirtualServer *default_s,
			   int client_socket, sockaddr_storage *client_addr) :
	_server(server),
	_default_server(default_s),
	_client_socket(client_socket),
	_addr(client_addr)
{
}

Client::Client(const Client &src) :
	_server(src._server),
	_default_server(src._default_server),
	_client_socket(src._client_socket)
{
}

Client &Client::operator=(const Client &src)
{
	if (this != &src) {}
	return *this;
}

bool Client::operator==(const Client &rhs) const
{
	return (_server == rhs._server && _default_server == rhs._default_server &&
			_client_socket == rhs._client_socket);
}

int Client::getSocket() const
{
	return _client_socket;
}

const ServerHost *Client::getHost() const
{
	return _host;
}

const std::string &Client::getRequest() const
{
	return _request;
}

const std::string &Client::getBody() const
{
	return _body;
}

void Client::handleRequest()
{
	Parser parser;
	parser.parseRequest(_request);

	Builder builder(_server, _default_server, parser);

	if (builder.getCode() != "200") {
		builder.findErrorpageLocationServer();
	}
	builder.returnParam();
	builder.setIndexOrReturnAutoindex();
	if (!builder.getFilename().empty() && builder.getFileExtension() == ".py") {

		return;
	}
	else if (!builder.getFilename().empty()) {
		builder.findFile();
	}
	builder.BuildResponse(_response);
}

Client::~Client()
{
	delete _addr;
}