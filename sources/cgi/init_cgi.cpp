/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cgi.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:16 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/31 15:19:41 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"
#include "client/Client.hpp"

using std::map;
using std::string;

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

void getClientInfo(const client::Client *client, std::vector< string > &env_vec)
{
	const sockaddr_storage *addr = client->getAddr();

	if (addr->ss_family == AF_INET) {
		const sockaddr_in *addr_in = reinterpret_cast< const sockaddr_in * >(
			addr); // TODO go to cast template
		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(addr_in->sin_addr), ip, INET_ADDRSTRLEN);
		env_vec.push_back("REMOTE_ADDR=" + string(ip));
		env_vec.push_back("REMOTE_PORT=" + ToString(ntohs(addr_in->sin_port)));
	}
	else if (addr->ss_family == AF_INET6) {
		const sockaddr_in6 *addr_in6 = reinterpret_cast< const sockaddr_in6 * >(
			addr); // TODO go to cast template
		char ip[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &(addr_in6->sin6_addr), ip, INET6_ADDRSTRLEN);
		env_vec.push_back("REMOTE_ADDR=" + string(ip));
		env_vec.push_back("REMOTE_PORT=" +
						  ToString(ntohs(addr_in6->sin6_port)));
	}
}

char **CGIHandler::createEnv(const VirtualServer  *server,
							 const client::Parser *parser,
							 const client::Client *client)
{
	string path_translated;

	std::vector< string > env_vec;
	{
		env_vec.push_back("AUTH_TYPE=");
		env_vec.push_back("CONTENT_LENGTH=" +
						  ToString(parser->getHeader("body").size()));
		env_vec.push_back("CONTENT_TYPE=" + parser->getHeader("Content-Type"));
		env_vec.push_back("GATEWAY_INTERFACE=CGI/1.1");
		env_vec.push_back("PATH_INFO=" + parser->getPathInfo());
		env_vec.push_back("PATH_TRANSLATED=" +
						  getTranslatedPath(parser, server));
		env_vec.push_back("QUERY_STRING=" + parser->getQuery());
		env_vec.push_back("REQUEST_METHOD=" + parser->getHeader("Method"));
		env_vec.push_back("REQUEST_URI=" + parser->getRequestedPath());
		env_vec.push_back("SERVER_NAME=" +
						  server->getParamValue("server_name"));
		env_vec.push_back("SERVER_PORT=" +
						  server->getParamPair("listen").second);
		env_vec.push_back("SERVER_PROTOCOL=" +
						  parser->getHeader("httpVersion"));
		env_vec.push_back("REDIRECT_STATUS=" + ToString(0));
		env_vec.push_back("HTTP_COOKIE=" + parser->getHeader("Cookie"));
		env_vec.push_back("SERVER_SOFTWARE=webserv/0.5");

		// TODO : check if it's a good idea to hardcode this
		env_vec.push_back("PHP_SELF=/register.php");
		env_vec.push_back("SCRIPT_NAME=/register.php");
		env_vec.push_back("SCRIPT_FILENAME=/home/jbrousse/Documents/GitHub/"
						  "WebServ/www/cgi/register.php");
		env_vec.push_back("DOCUMENT_ROOT=/home/jbrousse/Documents/GitHub/"
						  "WebServ/www/cgi");

		getClientInfo(client, env_vec);
	}

	char **envp = new char *[env_vec.size() + 1];
	for (size_t i = 0; i < env_vec.size(); ++i) {
		envp[i] = new char[env_vec[i].size() + 1];
		strcpy(envp[i], env_vec[i].c_str());
	}
	envp[env_vec.size()] = NULL;
	return envp;
}

char **CGIHandler::createArgv(const client::Builder *builder,
							  const char			*cgi_path,
							  const string			&file_extension)
{
	char **argv;
	if (file_extension == "php") {
		argv = new char *[4];

		argv[0] = new char[strlen(cgi_path) + 1];
		strcpy(argv[0], cgi_path);
		argv[1] = new char[25];
		strcpy(argv[1], "-d cgi.force_redirect=0");
		argv[2] = new char[builder->getPath().size() + 1];
		strcpy(argv[2], builder->getPath().c_str());
		argv[3] = NULL;
	}
	else {
		argv = new char *[3];
		argv[0] = new char[strlen(cgi_path) + 1];
		strcpy(argv[0], cgi_path);
		argv[1] = new char[builder->getPath().size() + 1];
		strcpy(argv[1], builder->getPath().c_str());
		argv[2] = NULL;
	}
	return argv;
}
