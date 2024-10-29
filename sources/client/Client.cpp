/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/29 16:40:19 by Monsieur_Ca      ###   ########.fr       */
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

namespace {
void setErrorCodeAndBuild(const std::string &code, client::Builder *builder,
						  std::string &response)
{
	builder->setCode(code);
	builder->findErrorPage();
	builder->BuildResponse(response);
}
} // namespace

int Client::CGIResponse()
{
	// TODO : verif body to send correct error

	int wait_ret = _cgi_handler->waitCGI();
	if (wait_ret == CGI_WAIT) {
		return CONTINUE;
	}
	else if (wait_ret == CGI_DONE) {
		int ret = 0;
		if ((ret = _cgi_handler->getStatus()) != 0) {
			if (WIFEXITED(ret) && WEXITSTATUS(ret) != 0) {
				LOG_WARNING("CGI return with exit status: " +
								ToString(WEXITSTATUS(ret)),
							_CSERVER);
			}
			else {
				LOG_WARNING("CGI interrupt by SIG : " + ToString(WTERMSIG(ret)),
							_CSERVER);
			}
		}
		else if ((ret = _cgi_handler->recvCGIResponse()) != SUCCESS) {
			LOG_WARNING("CGI recv failed: " + std::string(strerror(errno)),
						_CSERVER);
		}
		if (ret == 0) {
			try {
				_cgi_handler->adjustHeader(_response);
			} catch (const std::exception &e) {
				LOG_ERROR(e.what(), _CSERVER);
				setErrorCodeAndBuild("500", _builder, _response);
			}
		}
		else {
			setErrorCodeAndBuild("500", _builder, _response);
		}
	}
	else {
		setErrorCodeAndBuild("500", _builder, _response);
	}
	delete _cgi_handler;
	_cgi_handler = NULL;
	return FINISH;
}

int Client::handleResponse()
{
	if (_cgi_handler != NULL) {
		return CGIResponse();
	}
	_builder->BuildResponse(_response);
	return FINISH;
}

void Client::handleRequest()
{
	typedef void (Builder::*ptr)(int &);

	Parser parser;
	parser.parseRequest(_request);

	std::cout << "request : " << _request << std::endl;
	_builder = new Builder(_server, _default_server, parser);

	int state = DEFAULT;
	ptr tab[] = {
		&Builder::returnParam,
		&Builder::verifMethod,
		&Builder::setIndexOrAutoindex,
		&Builder::isCGI,
	};

	if (_builder->getCode() != "200") {
		state = B_ERROR;
	}
	for (int i = 0; i < 4 && state == DEFAULT; ++i) {
		(_builder->*tab[i])(state);
	}

	switch (state) {
		case B_ERROR: {
			_builder->findErrorPage();
			break;
		}
		case DELETE: {
			handleDeleteRequest(parser);
			break;
		}
		case POST: {
			std::cout << "CASE POST" << std::endl;
			int ret = handlePostRequest(parser);
			if (ret != 0) {
				_builder->setCode(ToString(ret));
				_builder->findErrorPage();
			}
			break;
		}
		case REDIRECT: {
			break;
		}
		case AUTOINDEX: {
			_builder->getAutoindex();
			break;
		}
		case INDEX: {
			int ret = _builder->readDataRequest();
			if (ret != 0) {
				LOG_WARNING("Error Accessing index file from : " +
								_builder->getPath(),
							_server);
				_builder->setCode((ret == ENOENT) ? "404" : "403");
				_builder->findErrorPage();
			}
			break;
		}
		case CGI: {
			try {
				_cgi_handler = new CGIHandler(this, &parser, _server, _builder);
				_cgi_handler->execute();
			} catch (const std::exception &e) {
				LOG_ERROR(e.what(), _server);
				_builder->setCode("500");
				_builder->findErrorPage();
			}
			break;
		}
		default: {
			_builder->findFile();
		}
	}
}

Client::~Client()
{
	delete _addr;
}