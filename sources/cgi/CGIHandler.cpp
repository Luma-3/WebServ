/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:42:29 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/29 15:57:38 by Monsieur_Ca      ###   ########.fr       */
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
	_argv = createArgv(builder);
	_envp = createEnv(server, parser, client);
	_body = parser->getHeader("body");
	_cgi = new char[strlen(CGI_PYTHON) +
					1]; // TODO : check extension and get _cgi from config file
	strcpy(_cgi, CGI_PYTHON);
}

CGIHandler::~CGIHandler()
{
	delete[] _argv;
	delete[] _envp;
	delete[] _cgi;
}