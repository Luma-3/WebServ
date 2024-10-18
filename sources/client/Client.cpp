/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/18 13:57:48 by jbrousse         ###   ########.fr       */
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
	std::cout << "Je suis dans le constructeur de Client" << std::endl;
}

Client::Client(const Client &src) :
	_server(src._server),
	_default_server(src._default_server),
	_client_socket(src._client_socket),
	_autoindex_parent_location(src._autoindex_parent_location)
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
	Parser	parser(_server, _default_server);
	parser.parseRequest(_request);
	
	Builder builder(_server, _default_server);

	builder.setPath(parser.getRequestedPath());
	builder.setFilename(parser.getFilename());

	if (parser.getCodeResponse() != "200") {
		builder.findErrorPage(parser);
	}
	else if (builder.returnParam(parser) == true) {
		builder.findFile(parser, _body);
	}

	builder.BuildResponse(parser);
	_response = builder.getResponse();
}

Client::~Client()
{
	delete _addr;
}