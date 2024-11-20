/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHost.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 16:43:56 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/20 18:39:47 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/ServerHost.hpp"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include "Logger.hpp"
#include "template/StringUtils.tpp"

using std::string;

ServerHost::ServerHost() : _default_vhost(NULL), _nbVhost(0), _socket(-1) {}

ServerHost::ServerHost(const string &host, const string &port) :
	_default_vhost(NULL),
	_nbVhost(0),
	_socket(-1)
{

	struct addrinfo	 hints = {};
	struct addrinfo *info = NULL;
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

struct addrinfo *ServerHost::setupSocket(const string &host, const string &port,
										 struct addrinfo *hints)
{
	struct addrinfo *info = NULL;
	const int status = getaddrinfo(host.c_str(), port.c_str(), hints, &info);

	if (status != 0) {
		throw InternalServerException("getaddrinfo failed on " + host + ": ",
									  __LINE__, __FILE__,
									  string(gai_strerror(status)));
	}
	_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (_socket == -1) {
		throw InternalServerException("socket failed: ", __LINE__, __FILE__,
									  string(strerror(errno)));
	}
	int val = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(int)) ==
		-1) {
		throw InternalServerException("setsockopt failed: ", __LINE__, __FILE__,
									  string(strerror(errno)));
	}
	if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1) {
		throw InternalServerException("fcntl failed: ", __LINE__, __FILE__,
									  string(strerror(errno)));
	}

	return info;
}

void ServerHost::bindAndListenSocket(struct addrinfo *info) const
{
	if (bind(_socket, info->ai_addr, info->ai_addrlen) == -1) {
		throw InternalServerException("bind failed: ", __LINE__, __FILE__,
									  string(strerror(errno)));
	}

	freeaddrinfo(info);

	if (listen(_socket, MAXREQUEST) == -1) {
		throw InternalServerException("listen failed: ", __LINE__, __FILE__,
									  string(strerror(errno)));
	}
}

const VirtualServer *ServerHost::getVhost(const string &host_name) const
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

void ServerHost::AddServer(const string &host_name, VirtualServer *server)
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

	const int client_socket = accept(_socket, (sockaddr *)&client_addr, &len);
	if (client_socket == -1) {
		throw InternalServerException("accept failed: ", __LINE__, __FILE__,
									  string(strerror(errno)));
	}

	return client_socket;
}

string ServerHost::recvRequest(int client_socket)
{
	char   *buff = new char[MAX_REQ_SIZE];
	string	request;
	ssize_t nb_bytes = 0;
	long	body_length = 0;
	bool	header = false;

	while (true) {

		memset(buff, 0, MAX_REQ_SIZE);

		nb_bytes = recv(client_socket, buff, MAX_REQ_SIZE, 0);
		if (nb_bytes == -1) {
			delete[] buff;
			throw RecvException("recv failed: ", __LINE__, __FILE__,
								string(strerror(errno)));
		}
		if (nb_bytes == 0) {
			delete[] buff;
			close(client_socket);
			throw RecvException("Client disconnected", __LINE__, __FILE__, "");
		}
		request.append(buff, static_cast< size_t >(nb_bytes));

		if (!header) {
			if (request.find("\r\n\r\n") != string::npos) {
				header = true;
				string header = request.substr(0, request.find("\r\n\r\n") + 4);

				size_t pos_length = header.find("Content-Length: ");
				if (pos_length != string::npos) {
					pos_length += 16; // NOLINT(*-magic-numbers)
					size_t pos_end = header.find("\r\n", pos_length);
					string length =
						header.substr(pos_length, pos_end - pos_length);
					body_length = atol(length.c_str());
				}
			}
		}
		if (header &&
			request.size() >= body_length + request.find("\r\n\r\n") + 4) {
			break;
		}

		if (nb_bytes < MAX_REQ_SIZE) {
			break;
		}
	}
	delete[] buff;
	return request;
}

void chunckResponse(int client_socket, const std::string &response)
{
	const size_t pos = response.find("\r\n\r\n") + 4;
	const string header = response.substr(0, pos);
	const string body = response.substr(pos);

	size_t len = body.size();

	send(client_socket, header.c_str(), header.size(), 0);

	size_t offset = 0;
	while (len > 0) {

		const size_t	  chunk = len > CHUNK_SIZE ? CHUNK_SIZE : len;
		std::stringstream ss;

		ss << std::hex << chunk;
		const string chunk_size = ss.str() + "\r\n";
		const string chunked_response =
			chunk_size + body.substr(offset, chunk) + "\r\n";

		if (send(client_socket, chunked_response.c_str(),
				 chunked_response.size(), 0) == -1) {
			throw InternalServerException("send failed: ", __LINE__, __FILE__,
										  string(strerror(errno)));
		}
		len -= chunk;
		offset += chunk;
	}
	const string end = "0\r\n\r\n";
	send(client_socket, end.c_str(), end.size(), 0);
}

void ServerHost::sendResponse(int client_socket, const string &response)
{
	if (response.empty()) {
		return;
	}

	if (response.find("Transfer-Encoding: chunked") != string::npos) {
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

	std::map< const string, const VirtualServer * >::iterator it =
		_vhost.begin();
	while (it != _vhost.end()) {
		delete it->second;
		++it;
	}
}