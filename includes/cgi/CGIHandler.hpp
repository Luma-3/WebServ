/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/21 15:12:59 by jbrousse         ###   ########.fr       */
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

#define TIMEOUT	   10
#define CGI_PYTHON "/usr/bin/python3"
#define CGI_PHP	   "/usr/bin/php-cgi" // TODO : check path
#define CGI_JS	   "/usr/bin/node"

class Client;

class CGIHandler
{
  private:
	pid_t  _pid;
	int	   _pipefd[2];
	char **_argv;
	char **_envp;
	char  *_cgi;
	// const client::Client *_client;
	// client::Parser		 *_parser;
	// const VirtualServer	 *_server;
	std::string _response;

	void createEnv();

	static char **createEnv(const VirtualServer	 *server,
							const client::Parser *parser,
							const client::Client *client);

	static char **createArgv(const client::Parser *parser,
							 const VirtualServer  *server);

		int childProcess();
	int parentProcess();

  public:
	CGIHandler(const client::Client *client, client::Parser *parser,
			   const VirtualServer *server);
	CGIHandler(const CGIHandler &src);
	CGIHandler &operator=(const CGIHandler &src);
	~CGIHandler();

	void execute();
	void waitCGI();
};

void adjustHeader(std::string &response);

#endif // CGI_HPP