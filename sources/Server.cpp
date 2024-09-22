/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:11:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/22 18:38:31 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(std::string servername, std::string hostname, std::string port) :
	_name(servername),
	_hostname(hostname),
	_port(port),
	_server_socket(socket(AF_INET, SOCK_STREAM, 0))
{
}

int Server::getNbBytes() const
{
	return (_nb_bytes);
}

int Server::getSocket() const
{
	return (_server_socket);
}

std::string Server::getName() const
{
	return (_name);
}

std::string Server::getHost() const
{
	return (_hostname);
}

std::string Server::getPort() const
{
	return (_port);
}

std::string Server::getRequest() const
{
	return (_request);
}

int Server::createSocket()
{
	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "Error on set nonblocking on " << _name << std::endl;
		return (FAILURE);
	}
	int val = 1;
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &val,
				   sizeof(int)) == -1) {
		std::cerr << "error on setting the port on reusable on " << _name
				  << ": " << strerror(errno) << ". socket value is "
				  << _server_socket << std::endl;
		return (FAILURE);
	}
#ifdef SO_REUSEPORT
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEPORT, &val,
				   sizeof(int)) == -1) {
		std::cerr << "error on setting the port on reusable on " << _name
				  << ": " << strerror(errno) << std::endl;
		return (FAILURE);
	}
#endif
	std::cout << "Server " << _name << " is launched !" << std::endl;
	if (_server_socket == -1) {
		std::cerr << "socket failed on " << _name << std::endl;
		return (FAILURE);
	}
	if (getaddrinfo(_hostname.c_str(), _port.c_str(), NULL, &_info) != 0) {
		std::cerr << "getaddrinfo failed on" << _name << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}

int Server::setSocket()
{
	if (bind(_server_socket, _info->ai_addr, _info->ai_addrlen) == -1) {
		std::cerr << _name << ": bind failed on " << _name << std::endl;
		return (FAILURE);
	}
	if (listen(_server_socket, 10) == -1) {
		return (FAILURE);
	}
	return (SUCCESS);
}

int Server::receiveRequest()
{
	char buff[MAX_REQ_SIZE];

	_new_socket = accept(_server_socket, NULL, NULL);
	if (_new_socket == -1) {
		std::cerr << "Error on awaiting connection (accept) on " << _name
				  << std::endl;
		return (FAILURE);
	}
	while (42) {
		for (int i = 0; i < MAX_REQ_SIZE; ++i) {
			buff[i] = 0;
		}
		_nb_bytes = recv(_new_socket, buff, MAX_REQ_SIZE, 0);
		if (_nb_bytes == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
			std::cerr << "Error on recv on " << _name << std::endl;
			close(_new_socket);
			return (FAILURE);
		} else if (_nb_bytes == 0) {
			std::cout << "client disconnected on " << _name << std::endl;
			close(_new_socket);
		}
		_request.append(buff, _nb_bytes);
		if (_nb_bytes < MAX_REQ_SIZE) {
			break;
		}
	}
	return (SUCCESS);
}

int Server::sendResponse()
{
	// the following is to replace with the response constructor
	if (_nb_bytes > 0) {
		std::cout << _request << std::endl;
		char repTest[110] = "HTTP/1.1 200 OK\nDate: Mon, 09 Sep 2024 12:00:00 "
							"GMT\nContent-Length: 13\nConnection: "
							"keep-alive\n\nca marche!!!!\n";
		if (send(_new_socket, repTest, strlen(repTest), 0) == -1) {
			std::cerr << "Error on sending response on " << _name << std::endl;
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

Server::~Server()
{
	close(_server_socket);
	freeaddrinfo(_info);
}
