/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/16 16:03:53 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <cerrno>
#include <cstring>
#include <map>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "client/Client.hpp"
#include "client/Parser.hpp"
#include "server/VirtualServer.hpp"

#define WRITE 1
#define READ  0

class Client;

struct exec_data {
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

// void executeCGI(char *cgi, char **argv, char **envp, std::string &response);
// void initCGI(const client::Parser &parser, const VirtualServer &server,
// 			 const client::Client &client, std::string &response);
#endif // CGI_HPP