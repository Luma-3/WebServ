/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:11:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/02 10:36:13 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

#include "server/ServerException.hpp"

#define MAXREQEST 10

Server::Server() :
	_autoindex(0),
	_server_socket(-1),
	// _client_socket(0),
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
	_deny_methods(vector_deep_copy(server->getDenyMethods())),
	_error_pages(vector_deep_copy(server->getErrorPages())),
	_locations(vector_deep_copy(server->getLocations())),

	_server_socket(socket(AF_INET, SOCK_STREAM, 0)),
	// _client_socket(-1),
	_nb_bytes(-1),
	_info()
{
	std::cout << "Server constructor" << std::endl;
	delete server;
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

	_server_socket(src._server_socket),
	// _client_socket(src._client_socket),
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
		// _client_socket = src._client_socket;
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

int Server::getSocket() const
{
	return (_server_socket);
}

// int Server::getNewSocket() const
// {
// 	return (_client_socket);
// }

int Server::getNbBytes() const
{
	return (_nb_bytes);
}

std::string Server::getRequest() const
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

const std::vector< const statement::ErrorPage * > &Server::getErrorPages() const
{
	return (_error_pages);
}

const std::vector< const statement::Location * > &Server::getLocations() const
{
	return (_locations);
}

// Faire ca dans le constructeur
int Server::createSocket()
{
	int				val = 1;
	struct addrinfo hints = {.ai_flags = AI_PASSIVE,
							 .ai_family = AF_INET,
							 .ai_socktype = SOCK_STREAM};

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
	// if (setsockopt(_server_socket, SOL_SOCKET, SO_KEEPALIVE, &val,
	// 			   sizeof(int)) == -1) {
	// 	throw InternalServerException(
	// 		"error on setting the port on reusable on " + _name + ": " +
	// 		strerror(errno));
	// }
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

// Faire ca dans le constructeur
int Server::setSocket()
{
	if (bind(_server_socket, _info->ai_addr, _info->ai_addrlen) == -1) {
		throw InternalServerException("bind failed on " + _name);
	}
	freeaddrinfo(_info);
	if (listen(_server_socket, MAXREQEST) == -1) {
		throw InternalServerException("listen failed on " + _name);
		return (FAILURE);
	}
	return (SUCCESS);
}

int Server::acceptRequest(int epfd)
{
	int val = 1;
	// sockaddr_in addr;
	// socklen_t	len = sizeof(addr);
	int client_socket;

	client_socket = accept(_server_socket, NULL, NULL);
	if (client_socket == -1) {
		throw InternalServerException(
			"Error on awaiting connection (accept) on" + _name);
	}

	// // plus besoin de ca car la copy que fait accept copy aussi les attributs
	// du
	// // socket
	// if (fcntl(_client_socket, F_SETFL, O_NONBLOCK) == -1) {
	// 	throw InternalServerException("Error on set nonblocking on " + _name);
	// }
	// // plus besoin de ca car la copy que fait accept copy aussi les attributs
	// du
	// // socket

	// if (setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &val,
	// 			   sizeof(int)) == -1) {
	// 	throw InternalServerException(
	// 		"error on setting the port on reusable on " + _name + ": " +
	// 		strerror(errno));
	// }
	// plus besoin de ca car la copy que fait accept copy aussi les attribut du
	// socket

	// if (setsockopt(_client_socket, SOL_SOCKET, SO_REUSEPORT, &val,
	// 			   sizeof(int)) == -1) {
	// 	throw InternalServerException(
	// 		"error on setting the port on reusable on " + _name + ": " +
	// 		strerror(errno));
	// }
	// // plus besoin de ca car la copy que fait accept copy aussi les
	// attributs du
	// // socket

	if (setsockopt(client_socket, SOL_SOCKET, SO_KEEPALIVE, &val,
				   sizeof(int)) == -1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _name + ": " +
			strerror(errno));
	}
	this->epolladd(epfd, client_socket);
	// socktab.push_back(_client_socket);
	return (client_socket);
}

// mettre dans le parser
// int Server::receiveRequest(int epfd, int client_socket)
// {
// 	char buff[MAX_REQ_SIZE];

// 	while (true) {
// 		for (int i = 0; i < MAX_REQ_SIZE; ++i) {
// 			buff[i] = 0;
// 		}
// 		_nb_bytes =
// 			static_cast< int >(recv(client_socket, buff, MAX_REQ_SIZE, 0));
// 		if (_nb_bytes == -1) {
// 			// voir si on peu pas le gerer par le flag d'erreur de epoll
// 			// dans le epollwait
// 			std::cerr << "Error on recv on " << _name << std::endl;
// 			std::cout << errno << std::endl;
// 			close(client_socket);
// 			this->epolldel(epfd, client_socket);
// 			client_socket = -1;
// 		} else if (_nb_bytes == 0) {
// 			// voir si on peu pas le gerer par le flag de deconnection de epoll
// 			// dans le epollwait
// 			std::cout << "client disconnected on " << _name << std::endl;
// 			close(client_socket);
// 			this->epolldel(epfd, client_socket);
// 			client_socket = -1;
// 		}
// 		std::cout << _nb_bytes << std::endl;
// 		std::cout << buff << std::endl;
// 		_request.append(buff, _nb_bytes);
// 		if (_nb_bytes < MAX_REQ_SIZE) {
// 			break;
// 		}
// 	}
// 	return (SUCCESS);
// }

int Server::sendResponse(const std::string &reponse, int client_socket)
{
	if (_nb_bytes > 0) {
		if (send(client_socket, reponse.c_str(), reponse.length(), 0) == -1) {
			std::cerr << "Error on sending response on " << _name << std::endl;
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

// Mettre ca dans le handler
void Server::epolladd(int epfd, int socket)
{
	struct epoll_event event = {.events = EPOLLIN | EPOLLRDHUP,
								.data = {.fd = socket}};
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket, &event) == -1) {
		throw InternalServerException("Error on epoll_ctl for EPOLLIN");
	}
}

// Mettre ca dans le handler

void Server::epollmod(int epfd, int socket, int flag)
{
	struct epoll_event event = {.events = flag, .data = {.fd = socket}};
	if (epoll_ctl(epfd, EPOLL_CTL_MOD, socket, &event) == -1) {
		throw InternalServerException("Error on epoll_ctl for EPOLLOUT");
	}
}

// Mettre ca dans le handler

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
	// close(_server_socket);
	// if (_client_socket != -1) {
	// 	close(_client_socket);
	// }

	std::vector< const statement::Location * >::const_iterator it =
		_locations.begin();

	while (it != _locations.end()) {
		delete *it;
		++it;
	}

	std::vector< const statement::ErrorPage * >::const_iterator it2 =
		_error_pages.begin();

	while (it2 != _error_pages.end()) {
		delete *it2;
		++it2;
	}
}
