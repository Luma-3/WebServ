/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:11:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/09 12:55:43 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <stdlib.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;

Server::Server() :
	_port("8080"),
	_server_socket(socket(AF_INET, SOCK_STREAM, 0))
{
	if (_server_socket == -1) {
		cerr << "socket failed" << endl;
		exit(1);
	}
	if (getaddrinfo("localhost", _port.c_str(), NULL, &_info) != 0) {
		cerr << "getaddrinfo failed" << endl;
		exit(1);
	}

	if (bind(_server_socket, _info->ai_addr, _info->ai_addrlen) == -1) {
		cerr << "bind failed" << endl;
		exit(1);
	}

	listen(_server_socket, 10);
}

void Server::ListenConnexion()
{
	int	  client_sock;
	int	  size = 100;
	char *buff = new char[size];
	int	  nb_bytes;

	client_sock = accept(_server_socket, NULL, NULL);
	nb_bytes = recv(client_sock, buff, size, 0);
	if (nb_bytes == -1) {
		cerr << "Error on recv" << endl;
	}
	cout << buff << endl;
}

Server::~Server()
{
	close(_server_socket);
	freeaddrinfo(_info);
}

//test commit