/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cgi.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:16 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/19 17:33:42 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"
#include "client/Client.hpp"

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
		const sockaddr_in *addr_in = (const sockaddr_in *)addr;
		char			   ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(addr_in->sin_addr), ip, INET_ADDRSTRLEN);
		env_vec.push_back("REMOTE_ADDR=" + string(ip));
		env_vec.push_back("REMOTE_PORT=" + ToString(ntohs(addr_in->sin_port)));
	}
	else if (addr->ss_family == AF_INET6) {
		const sockaddr_in6 *addr_in6 = (sockaddr_in6 *)addr;
		char				ip[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &(addr_in6->sin6_addr), ip, INET6_ADDRSTRLEN);
		env_vec.push_back("REMOTE_ADDR=" + string(ip));
		env_vec.push_back("REMOTE_PORT=" +
						  ToString(ntohs(addr_in6->sin6_port)));
	}
}

void CGIHandler::createEnv(const VirtualServer	 *server,
						   const client::Parser	 *parser,
						   const client::Client	 *client,
						   const client::Builder *builder)
{
	const string path_translated;

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
		env_vec.push_back("PHP_SELF=/" + builder->getFilename());
		env_vec.push_back("SCRIPT_NAME=/" + builder->getFilename());
		env_vec.push_back("SCRIPT_FILENAME=" + client->getValueEnv("PWD") +
						  '/' + getTranslatedPath(parser, server) +
						  builder->getFilename());
		env_vec.push_back("DOCUMENT_ROOT=" + client->getValueEnv("PWD") + '/' +
						  getTranslatedPath(parser, server));

		getClientInfo(client, env_vec);
	}

	for (std::vector< string >::iterator it = env_vec.begin();
		 it != env_vec.end(); ++it) {
		_envp.push_back(ft_strdup(it->c_str()));
	}
	_envp.push_back(NULL);
}

// void CGIHandler::handleUploadDir(const std::string &body)
// {
// 	const size_t pos = body.find("filename=\"") + 10;
// 	const string filename = body.substr(pos, body.find("\"", pos) - pos - 1);
// 	const size_t end = body.find("\"", pos) - 1;

// 	_body = body.substr(0, pos) + _upload_dir + filename + body.substr(end);
// }

void CGIHandler::createArgv(const client::Builder *builder)
{
	_argv.push_back(ft_strdup(_cgi.c_str()));
	_argv.push_back(ft_strdup(builder->getPath().c_str()));
	LOG_DEBUG("Try to Access CGI Script:" + builder->getPath());
	_argv.push_back(NULL);
}
