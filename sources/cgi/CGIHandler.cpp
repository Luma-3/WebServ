/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:42:29 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/21 14:55:20 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

CGIHandler::CGIHandler(const client::Client *client, client::Parser *parser,
					   const VirtualServer *server) :
	_pid(-1),
	_pipefd{-1, -1},
	_argv(NULL),
	_envp(NULL),
	_cgi(NULL)
{
	_argv = createArgv(parser, server);
	_envp = createEnv(server, parser, client);
	_cgi = new char[strlen(CGI_PYTHON) + 1]; // TODO : check extension
	strcpy(_cgi, CGI_PYTHON);
}