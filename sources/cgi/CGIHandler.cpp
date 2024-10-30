/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:42:29 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/30 10:52:56 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

CGIHandler::CGIHandler(const client::Client *client, client::Parser *parser,
					   const VirtualServer *server, client::Builder *builder) :
	_pid(-1),
	_argv(NULL),
	_envp(NULL),
	_cgi(NULL)
{
	_pipeIn[0] = -1;
	_pipeIn[1] = -1;
	_pipeOut[0] = -1;
	_pipeOut[1] = -1;
	_argv = NULL;

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

	_argv = createArgv(builder, _cgi);
	_envp = createEnv(server, parser, client);
	_body = parser->getHeader("body");
}

CGIHandler::~CGIHandler()
{
	if (_argv != NULL) {
		delete[] _argv;
	}
	delete[] _envp;
	delete[] _cgi;
}