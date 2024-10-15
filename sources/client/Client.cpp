/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/15 16:10:51 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

#include <exception>
#include <unistd.h>

using client::Client;

Client::Client() : _server(NULL), _default_server(NULL), _client_socket(-1) {}

Client::Client(const VirtualServer *server, const VirtualServer *default_s,
			   int client_socket) :
	_server(server),
	_default_server(default_s),
	_client_socket(client_socket)
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
	(void)src;
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

// void Client::sendResponseCGI()
// {
// 	if (!_header.empty()) {
// 		if (send(_client_socket, _header.c_str(), _header.size(), 0) == -1) {
// 			return;
// 		}
// 		_header.clear();
// 	}

// 	while (send(_client_socket, _body.c_str(), PACKET_SIZE, 0) > 0) {
// 		if (_body.size() > PACKET_SIZE) {
// 			_body = _body.substr(PACKET_SIZE);
// 		}
// 		else {
// 			_body.clear();
// 		}
// 	}
// 	_request.clear();
// }

void Client::handleRequest()
{
	Builder builder(_server, _default_server, this);
	Parser	parser(_server, _default_server);

	parser.parseRequest(_request);
	builder.BuildResponse(parser);
	_response = builder.getResponse();
}

Client::~Client() {}