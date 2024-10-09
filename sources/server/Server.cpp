/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:11:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 14:37:38 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

#include "Logger.hpp"
#include "server/ServerException.hpp"
#include "template/vector_deep_copy.tpp"

Server::Server() : _socket(-1), _autoindex(0) {}

Server::Server(const statement::Server *server, const Server *default_server) :
	_socket(socket(AF_INET, SOCK_STREAM, 0)),

	_name("default"),
	_hostname(server->getHost()),
	_port(server->getPort()),
	_default(default_server),
	_root(server->getRoot()),
	_index(server->getIndex()),
	_autoindex(server->getAutoindex()),
	_deny_methods(vector_deep_copy(server->getDenyMethods())),
	_error_pages(vector_deep_copy(server->getErrorPages())),
	_locations(vector_deep_copy(server->getLocations()))

{

	_logger =
		new Logger(server->getLog().getValue2(),
				   Logger::StringToLogLevel(server->getLog().getValue1()));
	delete server;
	_logger->log(INFO, "Server " + _name + " created");
	if (_socket == -1) {
		throw InternalServerException("socket failed on " + _name);
	}
	setSocketOptions();
	bindAndListenSocket();
}

void Server::setSocketOptions()
{
	int val = 1;
	_logger->log(INFO, "Setting socket options on " + _name);
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(int)) ==
		-1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _name + ": " +
			strerror(errno));
	}
	if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1) {
		throw InternalServerException("Error on set nonblocking on " + _name);
	}
}

void Server::bindAndListenSocket()
{
	struct addrinfo	 hints = {};
	struct addrinfo *info;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	_logger->log(INFO, "Binding and listening socket on " + _name);
	std::cout << "port: " << _port << std::endl;
	if (getaddrinfo(_hostname.c_str(), _port.c_str(), &hints, &info) != 0) {
		throw InternalServerException("getaddrinfo failed on" + _name);
	}
	if (bind(_socket, info->ai_addr, info->ai_addrlen) == -1) {
		throw InternalServerException("bind failed on " + _name);
	}
	freeaddrinfo(info);
	if (listen(_socket, MAXREQEST) == -1) {
		throw InternalServerException("listen failed on " + _name);
	}
}

Server::Server(const Server &src) :
	_socket(src._socket),

	_name(src._name),
	_hostname(src._hostname),
	_port(src._port),
	_root(src._root),
	_index(src._index),
	_autoindex(src._autoindex),
	_returns(src._returns),
	_deny_methods(src._deny_methods),
	_error_pages(vector_deep_copy(src._error_pages)),
	_locations(vector_deep_copy(src._locations))

{
}

Server &Server::operator=(const Server &src)
{
	if (this != &src) {
		return (*this);
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
	return (_socket);
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

const statement::ParamDouble &Server::getReturns() const
{
	return (_returns);
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

const Server *Server::getDefault() const
{
	return (_default);
}

int Server::acceptRequest() const
{
	int val = 1;
	int client_socket;

	client_socket = accept(_socket, NULL, NULL);
	if (client_socket == -1) {
		throw InternalServerException(
			"Error on awaiting connection (accept) on" + _name);
	}
	_logger->log(INFO, "Connection accepted on " + _name);

	if (setsockopt(client_socket, SOL_SOCKET, SO_KEEPALIVE, &val,
				   sizeof(int)) == -1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _name + ": " +
			strerror(errno));
	}
	_logger->log(INFO,
				 "Socket options set on New Connexion for Server " + _name);
	return (client_socket);
}

Server::~Server()
{

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
	delete _logger;
}
