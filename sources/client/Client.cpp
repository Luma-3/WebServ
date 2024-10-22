/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/23 01:16:35 by anthony          ###   ########.fr       */
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
	_addr(client_addr),
	_cgi_handler(NULL)
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

int Client::handleResponse()
{
	if (_cgi_handler != NULL) {
		if (_cgi_handler->waitCGI() == CGI_DONE) {
			_cgi_handler->recvCGIResponse();
			if (_cgi_handler->adjustHeader(_response) == FAILURE) {
				_builder->setCode("500");
				_builder->findErrorPage();
				_builder->BuildResponse(_response);
			}
			delete _cgi_handler;
			_cgi_handler = NULL;
			return FINISH;
		}
		return CONTINUE;
	}
	_builder->BuildResponse(_response);
	return FINISH;
}

void Client::handleRequest()
{
	typedef void (Builder::*ptr)(int &);

	Parser parser;
	parser.parseRequest(_request);

	_builder = new Builder(_server, _default_server, parser);

	std::cout << "REQUEST =" << _request << std::endl;
	int state = DEFAULT;
	ptr tab[] = {&Builder::returnParam, &Builder::verifDenyMethod,
				 &Builder::setIndexOrAutoindex, &Builder::isCGI};

	if (_builder->getCode() != "200") {
		state = ERROR;
	}

	for (int i = 0; i < 4 && state == DEFAULT; ++i) {
		(_builder->*tab[i])(state);
	}

	switch (state) {
		case ERROR: {
			std::cout << "ERROR" << std::endl;
			_builder->findErrorPage();
			break;
		}
		case REDIRECT: {
			std::cout << "REDIRECT" << std::endl;
			break;
		}
		case AUTOINDEX: {
			std::cout << "AUTOINDEX" << std::endl;
			_builder->getAutoindex();
			break;
		}
		case INDEX: {
			std::cout << "INDEX" << std::endl;
			int ret = _builder->readDataRequest();
			if (ret != 0) {
				_builder->setCode((ret == ENOENT) ? "404" : "403");
				_builder->findErrorPage();
			}
			break;
		}
		case CGI: {
			std::cout << "CGI" << std::endl;
			_cgi_handler = new CGIHandler(this, &parser, _server, _builder);
			_cgi_handler->execute();
			break;
		}
		default: {
			std::cout << "default" << std::endl;
			_builder->findFile();
		}
	}
}

Client::~Client()
{
	delete _addr;
}