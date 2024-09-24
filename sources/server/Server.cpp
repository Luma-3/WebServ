/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:11:21 by jbrousse          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2024/09/24 14:57:00 by jbrousse         ###   ########.fr       */
=======
/*   Updated: 2024/09/25 00:55:00 by anthony          ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

#include "server/ServerException.hpp"

#define MAXREQEST 10

Server::Server() :
	_autoindex(0),
	_server_socket(-1),
	_new_socket(0),
	_nb_bytes(0),
	_info()
{
}

Server::Server(const statement::Server *server) :
	_name("default"), // TODO : add name in parser
	_hostname(server->getHost()),
	_port(server->getPort()),
	_root(server->getRoot()),
	_index(server->getIndex()),
	_autoindex(server->getAutoindex()),
	_deny_methods(server->getDenyMethods()),
	_error_pages(server->getErrorPages()),
	_locations(server->getLocations()),

	_server_socket(socket(AF_INET, SOCK_STREAM, 0)),
	_new_socket(-1),
	_nb_bytes(-1),
	_info()
{
	// delete server;
	if (_server_socket == -1) {
		throw InternalServerException("socket failed on " + _name);
	}
}

Server::Server(const Server &src) :
	_name(src._name),
	_hostname(src._hostname),
	_port(src._port),
	_root(src._root),
	_index(src._index),
	_autoindex(src._autoindex),
	_deny_methods(src._deny_methods),
	_error_pages(src._error_pages),
	_locations(src._locations),

	_server_socket(src._server_socket),
	_new_socket(src._new_socket),
	_nb_bytes(src._nb_bytes),
	_request(src._request),
	_info(src._info)
{
}

Server &Server::operator=(const Server &src)
{
	if (this != &src) {
		_new_socket = src._new_socket;
		_nb_bytes = src._nb_bytes;
		_request = src._request;
		_info = src._info;
	}
	return (*this);
}

int Server::getSocket() const
{
	return (_server_socket);
}

int Server::getNbBytes() const
{
	return (_nb_bytes);
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

int Server::getNewSocket() const
{
	return (_new_socket);
}

int Server::createSocket()
{
	int val = 1;
	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1) {
		throw InternalServerException("Error on set nonblocking on " + _name);
	}
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &val,
				   sizeof(int)) == -1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _name + ": " +
			strerror(errno));
	}
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEPORT, &val,
				   sizeof(int)) == -1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _name + ": " +
			strerror(errno));
	}
	std::cout << "Server " << _name << " is launched !"
			  << std::endl; // TODO : go to logger
	if (_server_socket == -1) {
		throw InternalServerException("socket failed on " + _name);
	}
	if (getaddrinfo(_hostname.c_str(), _port.c_str(), NULL, &_info) != 0) {
		throw InternalServerException("getaddrinfo failed on" + _name);
	}
	return (SUCCESS);
}

int Server::setSocket()
{
	if (bind(_server_socket, _info->ai_addr, _info->ai_addrlen) == -1) {
		throw InternalServerException("bind failed on " + _name);
	}
	if (listen(_server_socket, MAXREQEST) == -1) {
		throw InternalServerException("listen failed on " + _name);
		return (FAILURE);
	}
	return (SUCCESS);
}

int setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		// perror("fcntl");
		// return -1;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

#include <sys/epoll.h>
int Server::receiveRequest(int &epfd)
{
	char buff[MAX_REQ_SIZE];

	std::cout << "SERVER REQUEST" << std::endl;
	_new_socket = accept(_server_socket, NULL, NULL);
	if (_new_socket == -1) {
		throw InternalServerException(
			"Error on awaiting connection (accept) on" + _name);
	}
	_new_socket = setNonBlocking(_new_socket);

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET; // Edge-triggered
	ev.data.fd = _new_socket;
	epoll_ctl(epfd, EPOLL_CTL_ADD, _new_socket, &ev);

	while (true) {
		std::cout << "Je rentre dans la boucle" << std::endl;
		for (int i = 0; i < MAX_REQ_SIZE; ++i) {
			buff[i] = 0;
		}
		_nb_bytes =
			static_cast< int >(recv(_new_socket, buff, MAX_REQ_SIZE, 0));

		std::cout << "Info receive" << std::endl;
		std::cout << "buffer = " << buff << std::endl;
		if (_nb_bytes == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
			std::cerr << "Error on recv on " << _name << std::endl;
			close(_new_socket);
			return (FAILURE);
		} else if (_nb_bytes == 0) {
			std::cout << "client disconnected on " << _name << std::endl;
			close(_new_socket);
			return (FAILURE);
		}
		_request.append(buff, _nb_bytes);
		if (_nb_bytes < MAX_REQ_SIZE) {
			break;
		}
	}
	return (SUCCESS);
}

<<<<<<< Updated upstream
int Server::sendResponse()
{
	// the following is to replace with the response constructor
	if (_nb_bytes > 0) {
		std::cout << _request << std::endl;
		const int rep_size = 110;
		char	  repTest[rep_size] =
			"HTTP/1.1 200 OK\nDate: Mon, 09 Sep 2024 12:00:00 "
			"GMT\nContent-Length: 13\nConnection: "
			"keep-alive\n\nca marche!!!!\n";
		if (send(_new_socket, repTest, strlen(repTest), 0) == -1) {
=======
std::string Server::receiveClientRequest(int &client_socket)
{
	char		buff[MAX_REQ_SIZE];
	std::string request;

	std::cout << "CLIENT REQUEST" << std::endl;
	while (true) {
		for (int i = 0; i < MAX_REQ_SIZE; ++i) {
			buff[i] = 0;
		}
		std::cout << "On attend la requete" << std::endl;
		_nb_bytes =
			static_cast< int >(recv(client_socket, buff, MAX_REQ_SIZE, 0));

		std::cout << "Info receive" << std::endl;
		std::cout << "buffer = " << std::endl;
		if (_nb_bytes == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
			std::cerr << "Error on recv on " << _name << std::endl;
			close(client_socket);
		} else if (_nb_bytes == 0) {
			std::cout << "client disconnected on " << _name << std::endl;
			close(client_socket);
		}
		request += buff;
		std::cout << "request" << request << std::endl;
		if (_nb_bytes < MAX_REQ_SIZE) {
			break;
		}
	}
	return (request);
}

int Server::sendResponse(const std::string &reponse, int socket)
{
	// the following is to replace with the response constructor
	if (_nb_bytes > 0) {
		// std::cout << _request << std::endl;
		// const int rep_size = 110;
		// char	  repTest[rep_size] =
		// 	"HTTP/1.1 200 OK\nDate: Mon, 09 Sep 2024 12:00:00 "
		// 	"GMT\nContent-Length: 13\nConnection: "
		// 	"keep-alive\n\nca marche!!!!\n";
		if (send(socket, reponse.c_str(), reponse.length(), 0) == -1) {
>>>>>>> Stashed changes
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