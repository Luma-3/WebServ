/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:42:29 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/19 10:16:49 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

#include "finder.hpp"

CGIHandler::CGIHandler(const client::Client *client, client::Parser *parser,
					   const VirtualServer *server, client::Builder *builder) :
	_env(client->getEnv()),
	_pid(-1),
	_pipeOut(),
	_pipeIn(),
	_status(CGI_WAIT)
{
	_pipeIn[0] = -1;
	_pipeIn[1] = -1;
	_pipeOut[0] = -1;
	_pipeOut[1] = -1;

	const std::string fileExtension = builder->getFileExtension();

	_cgi = findParam(fileExtension, builder->getRequestedPath(), server);
	if (_cgi.empty()) {
		_status = CGI_FAIL;
		throw std::runtime_error("CGI path not found for this extention: " +
								 fileExtension);
	}

	_upload_dir = findParam("upload_dir", builder->getRequestedPath(), server);

	createArgv(builder);
	createEnv(server, parser, client, builder);
	std::string request_body = parser->getHeader("body");
	if (request_body.find("multipart/form-data") != std::string::npos) {
		handleUploadDir(request_body);
	}
	else {
		_body = request_body;
	}
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