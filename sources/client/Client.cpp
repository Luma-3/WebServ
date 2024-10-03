/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/03 13:35:14 by jbrousse         ###   ########.fr       */
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

void Client::receiveRequest()
{
	char   *buff = new char[MAX_REQ_SIZE];
	ssize_t nb_bytes;

	while (true) {
		bzero(buff, MAX_REQ_SIZE);
		nb_bytes = recv(_client_socket, buff, MAX_REQ_SIZE, 0);
		if (nb_bytes == -1) {
			std::cerr << strerror(errno) << std::endl;
			throw std::runtime_error("Error on recv on " + _server->getName());
		}
		if (nb_bytes == 0) {
			break;
		}
		_request.append(buff, static_cast< size_t >(nb_bytes));
		if (nb_bytes < MAX_REQ_SIZE) {
			break;
		}
	}
	delete[] buff;
	std::cout << "balbalbnladf : " << _request << std::endl;
}

void Client::sendResponse()
{
	if (send(_client_socket, _response.c_str(), _response.size(), 0) == -1) {
		std::cerr << strerror(errno) << std::endl;
		// throw std::runtime_error("Error on send on " + _server->getName());
	}
	_request.clear();
}

void Client::handleRequest()
{
	Builder builder;
	Parser	parser(_server, _default_server);

	parser.parseRequest(_request);
	_response = builder.BuildResponse(parser, _server, _default_server);
}

Client::~Client() {}