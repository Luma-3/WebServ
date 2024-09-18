/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:56:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 22:49:45 by jdufour          ###   ########.fr       */
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

#include "Client.hpp"

#define SUCCESS 0
#define FAILURE 1

class Server
{
  protected:
	const std::string _name;
	const std::string _hostname;
	const std::string _port;
	const int		  _server_socket;
	int				  _new_socket;
	struct addrinfo	 *_info;
	char			 *_request;

  public:
	Server(std::string servername, std::string hostname, std::string port);

	int			getSocket(void) const;
	std::string getName(void) const;
	char	   *getRequest(void) const;

	int createSocket();
	int HandleConnexion();

	~Server();
};

#endif