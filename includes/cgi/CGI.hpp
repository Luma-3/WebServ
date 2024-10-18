/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/18 11:44:43 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "client/Client.hpp"
#include "client/Parser.hpp"
#include "server/VirtualServer.hpp"

#define WRITE 1
#define READ  0

#define TIMEOUT 10

class Client;

struct exec_data {
	pid_t				  pid;
	int					  pipefd[2];
	char				**argv;
	char				**envp;
	char				 *cgi;
	const client::Client *client;
	client::Parser		 *parser;
	const VirtualServer	 *server;
	std::string			 *response;
};

void initCGI(exec_data **info, const exec_data *hints);

int executeCGI(exec_data *info);

void recvCGIResponse(int fd, std::string *response);

void adjustHeader(std::string *response);

// void executeCGI(char *cgi, char **argv, char **envp, std::string &response);
// void initCGI(const client::Parser &parser, const VirtualServer &server,
// 			 const client::Client &client, std::string &response);

class CGI
{
  private:
	pid_t				  _pid;
	int					  _pipefd[2];
	char				**_argv;
	char				**_envp;
	char				 *_cgi;
	const client::Client *_client;
	client::Parser		 *_parser;
	const VirtualServer	 *_server;
	std::string			 *_response;

	void createEnv();

  public:
	CGI();
	CGI(const client::Client *client, client::Parser *parser,
		const VirtualServer *server, std::string *response);
	CGI(const CGI &src);
	CGI &operator=(const CGI &src);
	~CGI();

	void execute();
	void waitCGI();
};
#endif // CGI_HPP