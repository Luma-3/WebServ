/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:42:29 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/06 10:46:12 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

CGIHandler::CGIHandler(const client::Client *client, client::Parser *parser,
					   const VirtualServer *server, client::Builder *builder) :
	_env(client->getEnv()),
	_pid(-1),
	_cgi(NULL),
	_status(CGI_WAIT)
{
	_pipeIn[0] = -1;
	_pipeIn[1] = -1;
	_pipeOut[0] = -1;
	_pipeOut[1] = -1;

	std::string fileExtension = parser->getFileExtension();

	const Location *location = server->getLocation(parser->getRequestedPath());
	std::string		cgiPath;

	if (location != NULL) {
		cgiPath = location->getParamValue(fileExtension);
		if (cgiPath.empty()) {
			LOG_ERROR("CGI path not found for this extention: " + fileExtension,
					  _CSERVER);
			_status = CGI_FAIL;
			return;
		}
	}
	else {
		cgiPath = server->getParamValue(fileExtension);
		if (cgiPath.empty()) {
			LOG_ERROR("CGI path not found for this extention: " + fileExtension,
					  _CSERVER);
			_status = CGI_FAIL;
			return;
		}
	}

	_cgi = new char[cgiPath.length() + 1];
	strcpy(_cgi, cgiPath.c_str());
	// TODO: STRING AND VERIF ACCESS

	createArgv(builder);
	createEnv(server, parser, client);
	_body = parser->getHeader("body");
}

char *ft_strdup(const char *s)
{
	char *str = new char[strlen(s) + 1];
	strcpy(str, s);
	return str;
}

CGIHandler::~CGIHandler()
{
	delete[] _cgi;
	for (size_t i = 0; _envp[i] != NULL; i++) {
		delete[] _envp[i];
	}
	for (size_t i = 0; _argv[i] != NULL; i++) {
		delete[] _argv[i];
	}
}