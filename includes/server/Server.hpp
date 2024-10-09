/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:56:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 17:40:22 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_REQ_SIZE 100

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

#include "parser/statement/ErrorPage.hpp"
#include "parser/statement/Location.hpp"
#include "parser/statement/Server.hpp"

#define SUCCESS 0
#define FAILURE 1

class Server
{
  private:
	const std::string _name;
	const std::string _hostname;
	const std::string _port;

	const std::string _root;
	const std::string _index;
	const bool		  _autoindex;
	// TODO : add _return
	const std::vector< std::string >		   _deny_methods;
	const std::vector< statement::ErrorPage >  _error_pages;
	std::vector< const statement::Location * > _locations;

	const int				   _server_socket;
	std::vector< int >		   _client_socket;
	int						   _nb_bytes;
	std::vector< std::string > _request;
	struct addrinfo			  *_info;

  public:
	Server();
	Server(const std::string &servername, const std::string &hostname,
		   const std::string &port);
	Server(const statement::Server *server);
	Server(const Server &src);
	Server &operator=(const Server &src);

	std::string getName() const;
	std::string getHost() const;
	std::string getPort() const;

	const std::vector< int > &getClientSock() const;
	const int				 &getSocket() const;
	const int				 &getNbBytes() const;

	const std::vector< std::string > &getRequest() const;

	const std::string &getRoot() const;
	const std::string &getIndex() const;
	bool			   getAutoindex() const;

	const std::vector< std::string >				 &getDenyMethods() const;
	const std::vector< statement::ErrorPage >		 &getErrorPages() const;
	const std::vector< const statement::Location * > &getLocations() const;

	int createSocket();
	int setSocket();
	int acceptRequest(int epfd);
	int receiveRequest(int epfd, int socket, int i);
	int sendResponse(const std::string &response, int socket);

	void epolladd(int epfd, int socket);
	void epollmod(int epfd, int socket, int flag);
	void epolldel(int epfd, int socket);

	virtual ~Server();
};

#endif