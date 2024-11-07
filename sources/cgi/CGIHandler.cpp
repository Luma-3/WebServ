/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:42:29 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/07 13:50:51 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

CGIHandler::CGIHandler(const client::Client *client, client::Parser *parser,
					   const VirtualServer *server, client::Builder *builder) :
	_env(client->getEnv()),
	_pid(-1),
	_status(CGI_WAIT),
	_pipeIn(),
	_pipeOut()
{
	_pipeIn[0] = -1;
	_pipeIn[1] = -1;
	_pipeOut[0] = -1;
	_pipeOut[1] = -1;

	std::string fileExtension = parser->getFileExtension();

	const Location *location = server->getLocation(parser->getRequestedPath());

	if (location != NULL) {
		_cgi = location->getParamValue(fileExtension);
		if (_cgi.empty()) {
			LOG_ERROR("CGI path not found for this extention: " + fileExtension,
					  CSERVER);
			_status = CGI_FAIL;
			return;
		}
	}
	else {
		_cgi = server->getParamValue(fileExtension);
		if (_cgi.empty()) {
			LOG_ERROR("CGI path not found for this extention: " + fileExtension,
					  CSERVER);
			_status = CGI_FAIL;
			return;
		}
	}
	createArgv(builder);
	createEnv(server, parser, client);
	_body = parser->getHeader("body");
}

char *ft_strdup(const char *s)
{
	char *str = new char[strlen(s) + 1];
	strncpy(str, s, strlen(s) + 1);
	return str;
}

CGIHandler::~CGIHandler()
{
	for (std::vector< char * >::iterator it = _envp.begin(); it != _envp.end();
		 ++it) {
		if (*it != NULL) {
			delete[] *it;
		}
	}
	for (std::vector< char * >::iterator it = _argv.begin(); it != _argv.end();
		 ++it) {
		if (*it != NULL) {
			delete[] *it;
		}
	}
}