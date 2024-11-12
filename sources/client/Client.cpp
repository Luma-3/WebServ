/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/12 11:14:14 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

#include <exception>
#include <unistd.h>

using client::Client;

Client::Client(const VirtualServer *server, const VirtualServer *default_s,
			   int client_socket, sockaddr_storage *client_addr,
			   const char **envp) :
	_envp(envp),
	_server(server),
	_default_server(default_s),
	_host(NULL),
	_client_socket(client_socket),
	_addr(client_addr),
	_cgi_handler(NULL),
	_builder(NULL)
{
}

Client::Client(const Client &src) :
	_envp(src._envp),
	_server(src._server),
	_default_server(src._default_server),
	_host(src._host),
	_client_socket(src._client_socket),
	_addr(src._addr),
	_cgi_handler(src._cgi_handler),
	_builder(src._builder)
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
	int ret = 0;
	int wait_ret = _cgi_handler->waitCGI();

	if (wait_ret == CGI_WAIT) {
		return CONTINUE;
	}
	if (wait_ret == CGI_FAIL) {
		setErrorCodeAndBuild("500", _builder, _response);
		return returnAndDeleteCgi();
	}
	ret = _cgi_handler->getStatus();
	if (ret != 0) {
		if (WIFEXITED(ret) && WEXITSTATUS(ret) != 0) {
			LOG_WARNING("CGI return with exit status: " +
							ToString(WEXITSTATUS(ret)),
						CSERVER);
		}
		else {
			LOG_WARNING("CGI interrupt by SIG : " + ToString(WTERMSIG(ret)),
						CSERVER);
		}
	}

	if (_cgi_handler->recvCGIResponse() != SUCCESS) {
		LOG_WARNING("CGI recv failed: " + std::string(strerror(errno)),
					CSERVER);
	}

	try {
		_cgi_handler->adjustHeader(_response);
	} catch (const std::exception &e) {
		LOG_ERROR(e.what(), CSERVER);
		setErrorCodeAndBuild("500", _builder, _response);
	}
	return returnAndDeleteCgi();
}
int Client::returnAndDeleteCgi()
{
	delete _cgi_handler;
	_cgi_handler = NULL;
	delete _builder;
	_builder = NULL;
	return FINISH;
}
int Client::handleResponse()
{
	if (_cgi_handler != NULL) {
		return CGIResponse();
	}
	_builder->BuildResponse(_response);
	delete _builder;
	_builder = NULL;
	return FINISH;
}

void Client::handleRequest()
{
	typedef void (Builder::*ptr)(int &);

	Parser parser;
	parser.parseRequest(_request);

	_builder = new Builder(_server, _default_server, parser);

	int state = DEFAULT;
	ptr tab[] = {&Builder::returnParam, &Builder::setIndexOrAutoindex,
				 &Builder::verifMethod, &Builder::isCGI};

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
			handleDeleteRequest();
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
				if (_cgi_handler->getStatus() != CGI_FAIL) {
					_cgi_handler->execute();
				}
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

std::string Client::getValueEnv(const std::string &key) const
{
	for (int i = 0; _envp[i] != NULL; i++) {
		std::string envp_str(_envp[i]);
		size_t		equal_pos = envp_str.find_first_of('=');
		if (equal_pos != std::string::npos) {
			std::string env_key = envp_str.substr(0, equal_pos);
			if (env_key == key) {
				return envp_str.substr(equal_pos + 1);
			}
		}
	}
	return "";
}

Client::~Client()
{
	delete _addr;
	// if (_builder != NULL) {
	// 	delete _builder;
	// }
	// if (_cgi_handler != NULL) {
	// 	delete _cgi_handler;
	// }
}