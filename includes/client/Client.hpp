/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:22:15 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/27 11:06:57 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/socket.h>

#include "cgi/CGIHandler.hpp"
#include "client/Builder.hpp"
#include "client/Parser.hpp"
#include "server/ServerHost.hpp"
#include "server/VirtualServer.hpp"

#define MAX_REQ_SIZE 1024
#define PACKET_SIZE	 1024

#define DEFAULT	  0
#define INDEX	  1
#define CGI		  2
#define FILE	  3
#define B_ERROR	  4
#define AUTOINDEX 5
#define REDIRECT  6
#define DELETE	  7

#define FINISH	 1
#define CONTINUE 0

#define MAXBUFFERSIZE 100

class CGIHandler;

namespace client {

class Client
{
  private:
	const char		   **_envp;
	const ServerHost	*_serverHost;
	const VirtualServer *_server;
	const ServerHost	*_host;
	const int			 _client_socket;
	sockaddr_storage	*_addr;
	CGIHandler			*_cgi_handler;
	Builder				*_builder;

	std::string _request;
	std::string _response;
	std::string _header;
	std::string _body;

	void handleDeleteRequest();
	void removeFile(const std::string &full_path);
	void removeDirectory(const std::string &full_path);
	bool verifAccessInsideDirectory(const std::string &full_path);
	int	 returnAndDeleteCgi();
	int	 CGIResponse();
	void adjustCGIResponse();

  public:
	Client(const ServerHost *server_host, const VirtualServer *server,
		   int client_socket, sockaddr_storage *client_addr, const char **envp);
	Client(const Client &src);
	~Client();

	bool operator==(const Client &rhs) const;

	const ServerHost	   *getHost() const { return _host; };
	int						getSocket() const { return _client_socket; };
	const std::string	   &getRequest() const { return _request; };
	const std::string	   &getResponse() const { return _response; };
	const std::string	   &getBody() const { return _body; };
	const sockaddr_storage *getAddr() const { return _addr; };
	const VirtualServer	   *getServer() const { return _server; };
	const ServerHost	   *getServerHost() const { return _serverHost; };
	const char			  **getEnv() const { return _envp; };

	std::string getValueEnv(const std::string &key) const;
	void		setRequest(const std::string &request) { _request = request; };
	void setResponse(const std::string &response) { _response = response; };
	void setBuilder(Builder *builder) { _builder = builder; }

	void setVHost(const VirtualServer *server) { _server = server; };

	void handleRequest();
	int	 handleResponse();
};

} // namespace client

#endif // CLIENT_HPP
