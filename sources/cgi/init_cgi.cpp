/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cgi.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:16 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/15 16:51:45 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>
#include <sys/wait.h>

#include "cgi/CGI.hpp"
#include "client/Client.hpp"

using std::map;
using std::string;

#define CGI_PATH "/usr/bin/python3"

string getTranslatedPath(const client::Parser &parser,
						 const VirtualServer  &server)
{
	string			translated_path;
	const Location *location = server.getLocation(parser.getRequestedPath());

	if (location == NULL) {
		translated_path =
			server.getRoot(parser.getRequestedPath()) + parser.getPathInfo();
	}
	else {
		translated_path =
			location->getRoot(parser.getRequestedPath()) + parser.getPathInfo();
	}
	return translated_path;
}

char **createEnv(const VirtualServer &server, const client::Parser &parser,
				 const client::Client &client)
{
	string path_translated;
	(void)client; // TODO get client info

	std::vector< string > env_vec;
	{
		env_vec.push_back("AUTH_TYPE=");
		env_vec.push_back("CONTENT_LENGTH=");
		env_vec.push_back("CONTENT_TYPE=");
		env_vec.push_back("GATEWAY_INTERFACE=CGI/1.1");
		env_vec.push_back("PATH_INFO=" + parser.getPathInfo());
		env_vec.push_back("PATH_TRANSLATED=" +
						  getTranslatedPath(parser, server));
		env_vec.push_back("QUERY_STRING=");
		env_vec.push_back("REMOTE_ADDR=");
		env_vec.push_back("REMOTE_IDENT=");
		env_vec.push_back("REMOTE_USER=");
		env_vec.push_back("REQUEST_METHOD=" + parser.getHeaders().at("Method"));
		env_vec.push_back("REQUEST_URI=" + parser.getRequestedPath());
		env_vec.push_back("SCRIPT_NAME=" + parser.getRequestedPath());
		env_vec.push_back("SERVER_NAME=");
		env_vec.push_back("SERVER_PORT=");
		env_vec.push_back("SERVER_PROTOCOL=" +
						  parser.getHeaders().at("httpVersion"));
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

void initCGI(const client::Parser &parser, const VirtualServer &server,
			 const client::Client &client, std::string &response)
{
	string script = getTranslatedPath(parser, server) + parser.getFilename();
	char  *argv[] = {(char *)"/usr/bin/python3", (char *)script.c_str(), NULL};
	char **envp = createEnv(server, parser, client);

	executeCGI((char *)CGI_PATH, argv, envp, response);
}

char **convertMaptoTab(const std::map< std::string, std::string > &env)
{
	char **tab = new char *[env.size() + 1];
	int	   i = 0;

	for (std::map< std::string, std::string >::const_iterator it = env.begin();
		 it != env.end(); ++it) {
		tab[i] = new char[it->first.size() + it->second.size() + 2];
		tab[i] = strcpy(tab[i], (it->first + "=" + it->second).c_str());
		i++;
	}
	tab[i] = NULL;
	return tab;
}

void recvCGIResponse(int fd, std::string &response)
{
	char buffer[1024];
	int	 ret;

	while ((ret = read(fd, buffer, 1024)) > 0) {
		response.append(buffer, ret);
	}
}

void executeCGI(char *cgi, char **argv, char **envp, std::string &response)
{
	pid_t pid;
	int	  status;
	int	  pipefd[2];

	if (pipe(pipefd) == -1) {
		throw std::runtime_error("Pipe Error: " + string(strerror(errno)));
	}
	pid = fork();
	if (pid < 0) {
		throw std::runtime_error("Fork Error: " + string(strerror(errno)));
	}
	else if (pid == 0) {
		close(pipefd[READ]);
		dup2(pipefd[WRITE], STDOUT_FILENO);
		close(pipefd[WRITE]);
		if (execve(cgi, argv, envp) == -1) {
			throw std::runtime_error("Execve Error: " +
									 string(strerror(errno)));
		}
	}
	else {
		close(pipefd[WRITE]);
		dup2(pipefd[READ], STDIN_FILENO);
		close(pipefd[READ]);
		recvCGIResponse(STDIN_FILENO, response);
		waitpid(pid, &status, 0);
	}
}