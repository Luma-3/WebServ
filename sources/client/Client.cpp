/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/11 12:59:56 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

#include <exception>
#include <unistd.h>

using client::Client;

Client::Client() : _server(NULL), _default_server(NULL), _client_socket(-1) {}

Client::Client(const Server *server, const Server *default_s,
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

int Client::getSocket() const
{
	return _client_socket;
}

const Server *Client::getServer() const
{
	return _server;
}

const std::string &Client::getRequest() const
{
	return _request;
}

const std::string &Client::getBody() const
{
	return _body;
}

void Client::receiveRequest()
{
	char   *buff = new char[MAX_REQ_SIZE];
	ssize_t nb_bytes;

	while (true) {
		bzero(buff, MAX_REQ_SIZE);
		nb_bytes = recv(_client_socket, buff, MAX_REQ_SIZE, 0);
		if (nb_bytes == -1) {
			break;
		}
		_request.append(buff, static_cast< size_t >(nb_bytes));
		if (nb_bytes < MAX_REQ_SIZE) {
			break;
		}
		else if (_request.find("\r\n\r\n") != std::string::npos) {
			break;
		}
		nb_bytes = 0;
	}
	delete[] buff;
}

void Client::sendResponse()
{
	if (send(_client_socket, _response.c_str(), _response.size(), 0) == -1) {
		//	TODO Throw
		return;
	}
	_request.clear();
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
	Builder builder;
	Parser	parser(_server, _default_server);

	parser.parseRequest(_request);
	builder.BuildResponse(parser);
	_response = builder.getResponse();
}

Client::~Client() {}