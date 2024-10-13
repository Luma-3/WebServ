/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_Server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:11:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/12 17:25:37 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/old_Server.hpp"

#include "Logger.hpp"
#include "server/ServerException.hpp"
#include "template/vector_deep_copy.tpp"

old_Server::old_Server() : _socket(-1), _autoindex(0) {}

old_Server::Server(const statement::Server *server) :
	_ip(server->getIp()),
	_port(server->getPort()),
	_hostname(server->getHost()),
{
	_logger =
		new Logger(server->getLog().getValue2(),
				   Logger::StringToLogLevel(server->getLog().getValue1()));
	delete server;

	struct addrinfo	 hints = {};
	struct addrinfo *info;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status = getaddrinfo(_ip.c_str(), _port.c_str(), &hints, &info);
	if (status != 0) {
		throw InternalServerException("getaddrinfo failed on" + _hostname +
									  ": " + gai_strerror(status));
	}
	_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (_socket == -1) {
		throw InternalServerException("socket failed on " + _hostname + ": " +
									  strerror(errno));
	}
	setSocketOptions();

	if (bind(_socket, info->ai_addr, info->ai_addrlen) == -1) {
		throw InternalServerException("bind failed on " + _hostname + ": " +
									  strerror(errno));
	}
	freeaddrinfo(info);
	if (listen(_socket, MAXREQUEST) == -1) {
		throw InternalServerException("listen failed on " + _hostname + ": " +
									  strerror(errno));
	}
}

void old_Server::setSocketOptions()
{
	int val = 1;
	_logger->log(INFO, "Setting socket options on " + _hostname);
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(int)) ==
		-1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + _hostname + ": " +
			strerror(errno));
	}
	if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1) {
		throw InternalServerException("Error on set nonblocking on " +
									  _hostname + ": " + strerror(errno));
	}
}

void old_Server::bindAndListenSocket(struct addrinfo *info)
{
	_logger->log(INFO, "Binding and listening socket on " + _hostname);
	if (bind(_socket, info->ai_addr, info->ai_addrlen) == -1) {
		throw InternalServerException("bind failed on " + _hostname + ": " +
									  strerror(errno));
	}
	freeaddrinfo(info);
	if (listen(_socket, MAXREQUEST) == -1) {
		throw InternalServerException("listen failed on " + _hostname);
	}
}

old_Server::old_Server(const old_Server &src) :
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

old_Server &old_Server::operator=(const old_Server &src)
{
	if (this != &src) {
		return (*this);
	}
	return (*this);
}

std::string old_Server::getName() const
{
	return (_name);
}

std::string old_Server::getHost() const
{
	return (_hostname);
}

std::string old_Server::getPort() const
{
	return (_port);
}

int old_Server::getSocket() const
{
	return (_socket);
}

const std::string &old_Server::getRoot() const
{
	return (_root);
}

const std::string &old_Server::getIndex() const
{
	return (_index);
}

bool old_Server::getAutoindex() const
{
	return (_autoindex);
}

const statement::ParamDouble &old_Server::getReturns() const
{
	return (_returns);
}

const std::vector< std::string > &old_Server::getDenyMethods() const
{
	return (_deny_methods);
}

const std::vector< const statement::ErrorPage * > &
old_Server::getErrorPages() const
{
	return (_error_pages);
}

const std::vector< const statement::Location * > &
old_Server::getLocations() const
{
	return (_locations);
}

const old_Server *old_Server::getDefault() const
{
	return (_default);
}

int old_Server::acceptRequest() const
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

old_Server::~old_Server()
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
