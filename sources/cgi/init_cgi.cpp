/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cgi.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:16 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/16 16:10:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGI.hpp"
#include "client/Client.hpp"

using std::map;
using std::string;

#define CGI_PATH "/usr/bin/python3"

string getTranslatedPath(const client::Parser *parser,
						 const VirtualServer  *server)
{
	string			translated_path;
	const Location *location = server->getLocation(parser->getRequestedPath());

	if (location == NULL) {
		translated_path =
			server->getRoot(parser->getRequestedPath()) + parser->getPathInfo();
	}
	else {
		translated_path = location->getRoot(parser->getRequestedPath()) +
						  parser->getPathInfo();
	}
	return translated_path;
}

char **createEnv(const VirtualServer *server, const client::Parser *parser,
				 const client::Client *client)
{
	string path_translated;
	(void)client; // TODO get client info

	std::vector< string > env_vec;
	{
		env_vec.push_back("AUTH_TYPE=");
		env_vec.push_back("CONTENT_LENGTH=");
		env_vec.push_back("CONTENT_TYPE=");
		env_vec.push_back("GATEWAY_INTERFACE=CGI/1.1");
		env_vec.push_back("PATH_INFO=" + parser->getPathInfo());
		env_vec.push_back("PATH_TRANSLATED=" +
						  getTranslatedPath(parser, server));
		env_vec.push_back("QUERY_STRING=");
		env_vec.push_back("REMOTE_ADDR=");
		env_vec.push_back("REMOTE_IDENT=");
		env_vec.push_back("REMOTE_USER=");
		env_vec.push_back("REQUEST_METHOD=" +
						  parser->getHeaders().at("Method"));
		env_vec.push_back("REQUEST_URI=" + parser->getRequestedPath());
		env_vec.push_back("SCRIPT_NAME=" + parser->getRequestedPath());
		env_vec.push_back("SERVER_NAME=");
		env_vec.push_back("SERVER_PORT=");
		env_vec.push_back("SERVER_PROTOCOL=" +
						  parser->getHeaders().at("httpVersion"));
		env_vec.push_back("SERVER_SOFTWARE=webserv/0.5");
	}

	char **envp = new char *[env_vec.size() + 1];
	for (size_t i = 0; i < env_vec.size(); ++i) {
		envp[i] = new char[env_vec[i].size() + 1];
		strcpy(envp[i], env_vec[i].c_str());
	}
	envp[env_vec.size()] = NULL;
	return envp;
}

void initCGI(exec_data **info, const exec_data *hints)
{
	(*info) = new exec_data;
	(*info)->client = hints->client;
	(*info)->parser = hints->parser;
	(*info)->server = hints->server;
	(*info)->response = hints->response;

	char **argv = new char *[3];
	string script = getTranslatedPath(hints->parser, hints->server) +
					hints->parser->getFilename();

	argv[0] = new char[strlen(CGI_PATH) + 1];
	strcpy(argv[0], CGI_PATH);
	argv[1] = new char[script.size() + 1];
	strcpy(argv[1], script.c_str());
	argv[2] = NULL;

	char **envp = createEnv(hints->server, hints->parser, hints->client);

	char *cgi = new char[strlen(CGI_PATH) + 1];
	strcpy(cgi, CGI_PATH);

	(*info)->argv = argv;
	(*info)->envp = envp;
	(*info)->cgi = cgi;
}
