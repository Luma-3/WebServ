/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:11:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 18:09:29 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

#include "server/ServerException.hpp"

#define MAXREQEST 10

Server::Server() :
	_autoindex(0),
	_server_socket(-1),
	_client_socket(0),
	_nb_bytes(0),
	_info()
{
}

Server::Server(const std::string &servername, const std::string &host,
			   const std::string &port) :
	_name(servername),
	_hostname(host),
	_port(port),
	_autoindex(0),
	_server_socket(socket(AF_INET, SOCK_STREAM, 0)),
	_nb_bytes(-1)
{
	if (_server_socket == -1) {
		throw InternalServerException("socket failed on " + _name);
	}
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
	_client_socket(-1),
	_nb_bytes(-1),
	_info()
{
	// delete server;
	if (_server_socket == -1) {
		throw InternalServerException("socket failed on " + _name);
	}

	std::cout << "Server location: " << _locations[0]->getRoute() << std::endl;
	std::cout << "Server location: " << _locations[0]->getRoot() << std::endl;
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

	_server_socket(src._server_socket),
	_client_socket(src._client_socket),
	_nb_bytes(src._nb_bytes),
	_request(src._request),
	_info(src._info)
{

	std::cout << "Server copy constructor" << std::endl;
	std::vector< const statement::Location * >::const_iterator it =
		src._locations.begin();

	while (it != src._locations.end()) {
		_locations.push_back(new statement::Location(**it));
		++it;
	}
}

Server &Server::operator=(const Server &src)
{
	if (this != &src) {
		_client_socket = src._client_socket;
		_nb_bytes = src._nb_bytes;
		_request = src._request;
		_info = src._info;
	}
	return (*this);
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

const int &Server::getSocket() const
{
	return (_server_socket);
}

const std::vector< int > &Server::getClientSock() const
{
	return (_client_socket);
}

const int &Server::getNbBytes() const
{
	return (_nb_bytes);
}

const std::vector< std::string > &Server::getRequest() const
{
	return (_request);
}

const std::string &Server::getRoot() const
{
	return (_root);
}

const std::string &Server::getIndex() const
{
	return (_index);
}

bool Server::getAutoindex() const
{
	return (_autoindex);
}

const std::vector< std::string > &Server::getDenyMethods() const
{
	return (_deny_methods);
}

const std::vector< statement::ErrorPage > &Server::getErrorPages() const
{
	return (_error_pages);
}

const std::vector< const statement::Location * > &Server::getLocations() const
{
	return (_locations);
}

int Server::createSocket()
{
	int				val = 1;
	struct addrinfo hints = {.ai_flags = AI_PASSIVE};

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
	if (getaddrinfo(_hostname.c_str(), _port.c_str(), &hints, &_info) != 0) {
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

int Server::acceptRequest(int epfd)
{
	int			val = 1;
	sockaddr_in addr;
	socklen_t	len = sizeof(addr);
	int			tmp_sock;

	tmp_sock = accept(_server_socket, (sockaddr *)&addr, &len);
	if (tmp_sock == -1) {
		throw InternalServerException(
			"Error on awaiting connection (accept) on" + _name);
	}
	if (fcntl(tmp_sock, F_SETFL, O_NONBLOCK) == -1) {
		throw InternalServerException("Error on set nonblocking on " + _name);
	}
	if (setsockopt(tmp_sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) ==
		-1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _name + ": " +
			strerror(errno));
	}
	if (setsockopt(tmp_sock, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(int)) ==
		-1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _name + ": " +
			strerror(errno));
	}
	if (setsockopt(tmp_sock, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(int)) ==
		-1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _name + ": " +
			strerror(errno));
	}
	_client_socket.push_back(tmp_sock);
	epolladd(epfd, tmp_sock);
	return (SUCCESS);
}

int Server::receiveRequest(int epfd, int socket, int req_nb)
{
	char buff[MAX_REQ_SIZE] = {0};

	if (static_cast<unsigned long>(req_nb) >= _request.size())
		_request.push_back(buff);
	_nb_bytes = static_cast< int >(recv(socket, buff, MAX_REQ_SIZE, 0));
	_request[req_nb].append(buff);
	if (_nb_bytes == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
		std::cerr << "Error on recv on " << _name << std::endl;
		std::cout << errno << std::endl;
		_request[req_nb].clear();
		this->epolldel(epfd, socket);
		close(socket);
		socket = -1;
	} else
	if (_nb_bytes == 0) {
		std::cout << "client disconnected on " << _name << std::endl;
		close(socket);
		_request[req_nb].clear();
		this->epolldel(epfd, socket);
		socket = -1;
	} else {
		std::cout << _nb_bytes << std::endl;
		std::cout << buff << std::endl;
	}
	return (SUCCESS);
}

int Server::sendResponse(const std::string &reponse, int socket)
{
	int sent = 0;
	std::cout << "sent = " << sent << std::endl;

	while (sent < static_cast< int >(reponse.length())) {
		if (_nb_bytes >= 0) {
			sent += send(socket, reponse.c_str() + sent, MAX_REQ_SIZE, 0);
			std::cout << "sent = " << sent << std::endl;
			if (sent < 0) {
				std::cerr << "Error on sending response on " << _name
						  << std::endl;
				return (FAILURE);
			}
		}
	}
	return (SUCCESS);
}

void Server::epolladd(int epfd, int socket)
{
	struct epoll_event event = {.events = EPOLLIN, .data = {.fd = socket}};
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket, &event) == -1) {
		throw InternalServerException("Error on epoll_ctl for EPOLLIN");
	}
}

void Server::epollmod(int epfd, int socket, int flag)
{
	struct epoll_event event = {.events = flag, .data = {.fd = socket}};
	if (epoll_ctl(epfd, EPOLL_CTL_MOD, socket, &event) == -1) {
		throw InternalServerException("Error on epoll_ctl for EPOLLOUT");
	}
}

void Server::epolldel(int epfd, int socket)
{
	if (epoll_ctl(epfd, EPOLL_CTL_DEL, socket, NULL) == -1) {
		throw InternalServerException("Error on epoll_ctl for EPOLLDEL");
	}
}

Server::~Server()
{
	// if (_info) {
	// 	freeaddrinfo(_info);
	// 	_info = NULL;
	// } TODO: handle inheritent destructor
	close(_server_socket);
}
