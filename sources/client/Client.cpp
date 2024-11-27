/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:30:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/27 11:06:04 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

#include <exception>
#include <unistd.h>

using client::Client;

Client::Client(const ServerHost *server_host, const VirtualServer *server,
			   int client_socket, sockaddr_storage *client_addr,
			   const char **envp) :
	_envp(envp),
	_serverHost(server_host),
	_server(server),
	_host(NULL),
	_client_socket(client_socket),
	_addr(client_addr),
	_cgi_handler(NULL),
	_builder(NULL)
{
}

Client::Client(const Client &src) :
	_envp(src._envp),
	_serverHost(src._host),
	_server(src._server),
	_host(src._host),
	_client_socket(src._client_socket),
	_addr(src._addr),
	_cgi_handler(src._cgi_handler),
	_builder(src._builder)
{
}

bool Client::operator==(const Client &rhs) const
{
	return (_server == rhs._server && _client_socket == rhs._client_socket);
}

namespace {
void setErrorCodeAndBuild(const std::string &code, client::Builder *builder,
						  std::string &response)
{
	builder->setCode(code);
	builder->findErrorPage();
	builder->BuildResponse(response);
}

void setErrorCodeAndFindPage(const std::string &code, client::Builder *builder)
{
	builder->setCode(code);
	builder->findErrorPage();
}

bool verifCGIExit(CGIHandler *_cgi_handler, client::Builder *builder,
				  std::string &response)
{
	std::string w_msg;
	std::string return_code;

	const int ret = _cgi_handler->getStatus();

	if (!WIFEXITED(ret)) {
		if (WIFSIGNALED(ret)) {
			if (WTERMSIG(ret) == SIGALRM) {
				w_msg = "CGI process timed out (SIGALRM)";
				return_code = "504";
			}
			else {
				w_msg = "CGI interrupt by SIG : " + ToString(WTERMSIG(ret));
				return_code = "502";
			}

			w_msg = (w_msg.empty()) ? ("WTF ?! CGI doesn't exit properly " +
									   ToString(WIFEXITED(ret)))
									: w_msg;

			return_code = (return_code.empty()) ? "502" : return_code;

			LOG_WARNING(w_msg);
			setErrorCodeAndBuild(return_code, builder, response);
			return true;
		}
		if (WEXITSTATUS(ret) != 0) {
			LOG_WARNING("CGI return with exit status: " +
						ToString(WEXITSTATUS(ret)));
			return false;
		}
	}
	return false;
}

} // namespace

int Client::CGIResponse()
{
	const int wait_ret = _cgi_handler->waitCGI();

	if (wait_ret == CGI_WAIT) {
		return CONTINUE;
	}

	if (wait_ret == CGI_FAIL) {
		setErrorCodeAndBuild("500", _builder, _response);
		return returnAndDeleteCgi();
	}

	if (verifCGIExit(_cgi_handler, _builder, _response)) {
		return returnAndDeleteCgi();
	}

	if (_cgi_handler->recvCGIResponse(_response) != SUCCESS) {
		LOG_WARNING("CGI recv failed: " + std::string(strerror(errno)));
		setErrorCodeAndBuild("502", _builder, _response);
		return returnAndDeleteCgi();
	}

	adjustCGIResponse();

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
		_response.clear();
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

	_builder = new Builder(_server, parser);

	int		  state = DEFAULT;
	const int ptr_tab_size = 5;
	const ptr tab[ptr_tab_size] = {
		&Builder::verifyMethodsAndBodySize, &Builder::returnParam,
		&Builder::validateIndexDir, &Builder::isCGI, &Builder::isDelete};

	if (_builder->getCode() != "200") {
		state = B_ERROR;
	}

	for (int i = 0; i < ptr_tab_size && state == DEFAULT; ++i) {
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
			_builder->Autoindex();
			break;
		}
		case CGI: {
			try {
				_cgi_handler = new CGIHandler(this, &parser, _server, _builder);
				if (_cgi_handler->getStatus() != CGI_FAIL) {
					_cgi_handler->execute();
				}
			} catch (const std::exception &e) {
				LOG_ERROR(e.what());
				setErrorCodeAndFindPage("500", _builder);
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

		const std::string envp_str(_envp[i]);
		const size_t	  equal_pos = envp_str.find_first_of('=');

		if (equal_pos != std::string::npos) {
			const std::string env_key = envp_str.substr(0, equal_pos);
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
	delete _builder;
	delete _cgi_handler;
	if (_client_socket != -1) {
		close(_client_socket);
	}
}
