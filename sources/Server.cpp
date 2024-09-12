/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:11:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/12 23:38:04 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

using std::cerr;
using std::cout;
using std::endl;

Server::Server( std::string servername, std::string hostname, std::string port) : 
	_name(servername),
	_hostname(hostname),
	_port(port),
	_server_socket(socket(AF_INET, SOCK_STREAM, 0)) {}

int	Server::getSocket( void) const
{
	return (_server_socket);
}

std::string	Server::getName( void) const
{
	return (_name);
}

void	Server::createSocket()
{
	// non-blocking flag added to socket
	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		cerr << "Error on set nonblocking" << endl;
		exit(1);
	}
	int	val = 1;
	// make sure the port is not BURNED TO ASHES (otherwise it will take some time for the used port to be reusable)
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) == -1)
	{
		cerr << "Error on setting the port on reusable : " << strerror(errno) << ". socket value is " << _server_socket << endl;
		exit(1);
	}
	#ifdef SO_REUSEPORT
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(int)) == -1)
	{
		cerr << "Error on setting the port on reusable : " << strerror(errno) << endl;
		exit(1);
	}
	#endif
	cout << "Server " << _name << " is launched !" << endl;
	if (_server_socket == -1) 
	{
		cerr << "socket failed" << endl;
		exit(1);
	}
	if (getaddrinfo(_hostname.c_str(), _port.c_str(), NULL, &_info) != 0) 
	{
		cerr << "getaddrinfo failed" << endl;
		exit(1);
	}

	if (bind(_server_socket, _info->ai_addr, _info->ai_addrlen) == -1) 
	{
		cerr << _name << ": bind failed" << endl;
		exit(1);
	}

	if (listen(_server_socket, 10) == -1)
		exit(1);	
}

void Server::HandleConnexion()
{
	int		size = 100;
	char	*buff = new char[size];
	int		nb_bytes;

	memset(buff, 0, 100);
	_new_socket = accept(_server_socket, NULL, NULL);
	nb_bytes = recv(_new_socket, buff, size, 0);
	if (nb_bytes == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
	{
		cerr << "Error on recv" << endl;
		close(_new_socket);
	}
	else if (nb_bytes > 0)
	{
		cout << buff << endl;

		// this will be to uncomment and adjust with the client functions
		// Client	serverClient = Client(buff);
		// serverClient.HandleRequest();
		// send(_new_socket, serverClient.getResponse().c_str(), strlen(serverClient.getResponse().c_str()), 0);
		char	repTest[110] = "HTTP/1.1 200 OK\nDate: Mon, 09 Sep 2024 12:00:00 GMT\nContent-Length: 13\nConnection: keep-alive\n\nca marche!!!!\n";
		send(_new_socket, repTest, strlen(repTest), 0);
	}
	else if (!nb_bytes)
	{
		std::cout << "client disconnected" << endl;
		close(_new_socket);
	}
}

Server::~Server()
{
	close(_server_socket);
	freeaddrinfo(_info);
}
