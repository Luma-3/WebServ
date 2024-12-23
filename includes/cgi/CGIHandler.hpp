/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/27 11:06:51 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
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
#include "Logger.hpp"
#include "server/VirtualServer.hpp"

#define WRITE 1
#define READ  0

#define CGI_DONE 1
#define CGI_WAIT 0
#define CGI_FAIL (-1)

#define TIMEOUT 15

#define FAILURE 1
#define SUCCESS 0

#define BUFFER_SIZE 1024
#define STATUS_POS	9

namespace client {
class Client;
class Builder;

} // namespace client

class CGIHandler
{
  private:
	const char			**_env;
	pid_t				  _pid;
	int					  _pipeOut[2];
	int					  _pipeIn[2];
	std::vector< char * > _envp;
	std::vector< char * > _argv;
	std::string			  _cgi;
	std::string			  _response;
	int					  _status;
	std::string			  _body;
	std::string			  _path;
	std::string			  _filename;

	void createEnv(const VirtualServer *server, const client::Parser *parser,
				   const client::Client	 *client,
				   const client::Builder *builder);

	void createArgv(const client::Builder *builder);

	int childProcess();
	int parentProcess();

  public:
	CGIHandler(const client::Client *client, client::Parser *parser,
			   const VirtualServer *server, client::Builder *builder);
	~CGIHandler();

	int execute();
	int waitCGI();
	int recvCGIResponse(std::string &response);
	int getStatus() const { return _status; };
};

char *ft_strdup(const char *s);

#endif // CGI_HPP
