/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHost.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 16:43:56 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/22 10:58:36 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/ServerHost.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include "server/ServerException.hpp"

ServerHost::ServerHost() : _default_vhost(NULL), _nbVhost(0), _socket(-1) {}

ServerHost::ServerHost(const std::string &host, const std::string &port) :
	_default_vhost(NULL),
	_nbVhost(0),
	_socket(-1)
{
	std::cout << "ServerHost created" << std::endl;

	struct addrinfo	 hints = {};
	struct addrinfo *info;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	info = setupSocket(host, port, &hints);
	bindAndListenSocket(info);
}

ServerHost::ServerHost(const ServerHost &src) :
	_default_vhost(src._default_vhost),
	_vhost(src._vhost),
	_nbVhost(src._nbVhost),
	_socket(src._socket)
{
}

ServerHost &ServerHost::operator=(const ServerHost &rhs)
{
	if (this != &rhs) {
		_default_vhost = rhs._default_vhost;
		_vhost = rhs._vhost;
		_nbVhost = rhs._nbVhost;
		_socket = rhs._socket;
	}
	return *this;
}

bool ServerHost::operator==(const ServerHost &rhs) const
{
	return _default_vhost == rhs._default_vhost && _vhost == rhs._vhost &&
		   _nbVhost == rhs._nbVhost && _socket == rhs._socket;
}

struct addrinfo *ServerHost::setupSocket(const std::string &host,
										 const std::string &port,
										 struct addrinfo   *hints)
{
	struct addrinfo *info;
	int status = getaddrinfo(host.c_str(), port.c_str(), hints, &info);
	if (status != 0) {
		throw InternalServerException("getaddrinfo failed on" + host + ": " +
									  gai_strerror(status));
	}
	_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (_socket == -1) {
		throw InternalServerException("socket failed on " + host + ": " +
									  strerror(errno));
	}
	int val = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(int)) ==
		-1) {
		throw InternalServerException(
			"error on setting the port on reusable on " + host + ": " +
			strerror(errno));
	}
	if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1) {
		throw InternalServerException("Error on set nonblocking on " + host +
									  ": " + strerror(errno));
	}

	return info;
}

void ServerHost::bindAndListenSocket(struct addrinfo *info)
{
	if (bind(_socket, info->ai_addr, info->ai_addrlen) == -1) {
		throw InternalServerException("bind failed on " +
									  std::string(strerror(errno)));
	}
	freeaddrinfo(info);
	if (listen(_socket, MAXREQUEST) == -1) {
		throw InternalServerException("listen failed: " +
									  std::string(strerror(errno)));
	}
}

const VirtualServer *ServerHost::getVhost(const std::string &host_name) const
{
	try {
		return _vhost.at(host_name);
	} catch (std::out_of_range &e) {
		return NULL;
	}
}

const VirtualServer *ServerHost::getDefaultVhost() const
{
	return _default_vhost;
}

void ServerHost::AddServer(std::string host_name, VirtualServer *server)
{
	if (_nbVhost == 0) {
		_default_vhost = server;
	}
	_vhost[host_name] = server;
	_nbVhost++;
}

int ServerHost::acceptClient(sockaddr_storage *client_addr) const
{
	socklen_t len = sizeof(sockaddr_storage);
	int		  client_socket = accept(_socket, (sockaddr *)client_addr, &len);
	if (client_socket == -1) {
		throw InternalServerException("accept failed: " +
									  std::string(strerror(errno)));
	}
	return client_socket;
}

std::string ServerHost::recvRequest(int client_socket)
{
	char	   *buff = new char[MAX_REQ_SIZE];
	std::string request;
	ssize_t		nb_bytes;

	while (true) {
		bzero(buff, MAX_REQ_SIZE);
		nb_bytes = recv(client_socket, buff, MAX_REQ_SIZE, 0);
		if (nb_bytes == -1) {
			break;
		}
		request.append(buff, static_cast< size_t >(nb_bytes));
		if (nb_bytes < MAX_REQ_SIZE) {
			break;
		}
		else if (request.find("\r\n\r\n") != std::string::npos) {
			break;
		}
		nb_bytes = 0;
	}
	delete[] buff;
	return request;
}

void chunckResponse(int client_socket, const std::string &response)
{
	std::cout << "Response: " << response << std::endl;
	size_t pos = response.find("\r\n\r\n") + 4;

	std::string header = response.substr(0, pos);
	std::string body = response.substr(pos);
	size_t		len = body.size();

	std::cout << "Chunked Header: " << header << std::endl;

	send(client_socket, header.c_str(), header.size(), 0);

	size_t offset = 0;
	while (len > 0) {
		size_t			  chunk = len > CHUNK_SIZE ? CHUNK_SIZE : len;
		std::stringstream ss;
		ss << std::hex << chunk;
		std::string chunk_size = ss.str() + "\r\n";
		std::string chunked_response =
			chunk_size + body.substr(offset, chunk) + "\r\n";
		std::cout << "Chunked Response: " << chunked_response << std::endl;
		send(client_socket, chunked_response.c_str(), chunked_response.size(),
			 0);
		len -= chunk;
		offset += chunk;
	}
	send(client_socket, "0\r\n\r\n", 5, 0);
}

void ServerHost::sendResponse(int client_socket, const std::string &response)
{
	if (response.empty()) {
		return;
	}

	if (response.find("Content-Length: ") == std::string::npos) {
		chunckResponse(client_socket, response);
	}
	else {
		send(client_socket, response.c_str(), response.size(), 0);
	}
}

ServerHost::~ServerHost()
{
	if (_socket != -1) {
		close(_socket);
	}

	std::map< const std::string, const VirtualServer * >::iterator it =
		_vhost.begin();
	while (it != _vhost.end()) {
		delete it->second;
		++it;
	}
}