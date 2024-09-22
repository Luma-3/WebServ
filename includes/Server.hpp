/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:56:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/22 18:38:05 by jdufour          ###   ########.fr       */
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
#include <unistd.h>

#define SUCCESS 0
#define FAILURE 1

class Server
{
  private:
	const std::string _name;
	const std::string _hostname;
	const std::string _port;
	const int		  _server_socket;
	int				  _new_socket;
	int				  _nb_bytes;
	std::string		  _request;
	struct addrinfo	 *_info;

  public:
	Server(std::string servername, std::string hostname, std::string port);

	int			getSocket() const;
	int			getNbBytes() const;
	std::string getName() const;
	std::string getHost() const;
	std::string getPort() const;
	std::string getRequest() const;

	int createSocket();
	int setSocket();
	int receiveRequest();
	int sendResponse();

	~Server();
};

#endif