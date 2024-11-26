/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/11/26 10:29:27 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Handler.hpp"

#include <algorithm>
#include <map>
#include <netdb.h>
#include <sys/socket.h>

#include "client/Builder.hpp"
#include "client/Parser.hpp"
#include "finder.hpp"
#include "server/ServerException.hpp"
#include "server/ServerHost.hpp"

using client::Builder;
using client::Client;
using client::Parser;
using std::string;
using std::vector;

volatile int g_sig = 0;


namespace {
string getIP(const struct addrinfo *addr)
{
	char ip[INET6_ADDRSTRLEN];
	if (addr->ai_family == AF_INET) {
		const sockaddr_in *addr_in = (struct sockaddr_in *)(addr->ai_addr);
		inet_ntop(AF_INET, &(addr_in->sin_addr), ip, INET_ADDRSTRLEN);
	}
	else if (addr->ai_family == AF_INET6) {
		const sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)(addr->ai_addr);
		inet_ntop(AF_INET6, &(addr_in6->sin6_addr), ip, INET6_ADDRSTRLEN);
	}
	return string(ip);
}
} // namespace

Handler::Handler(const vector< VirtualServer * > &servers, const char **envp) :
	_envp(envp),
	_epfd(epoll_create1(0)),
	_nbServ(0)
{

	if (_epfd == -1) {
		throw InternalServerException("epoll_create", __LINE__, __FILE__,
									  std::string(strerror(errno)));
	}

	vector< VirtualServer * >::const_iterator it = servers.begin();

	struct addrinfo *info = NULL;
	struct addrinfo	 hints = {};

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	while (it != servers.end()) {
		const Param *listen = (*it)->getParam("listen");

		const int status =
			getaddrinfo(listen->getPair().first.c_str(),
						listen->getPair().second.c_str(), &hints, &info);

		if (status != 0) {
			throw InternalServerException(
				"getaddrinfo failed on " +
					std::string(listen->getPair().first) + ": ",
				__LINE__, __FILE__, string(gai_strerror(status)));
		}

		const string hostkey =
			getIP(info) + string(":") + listen->getPair().second;

		if (_hostptofd.find(hostkey) == _hostptofd.end()) {

			ServerHost *host = new ServerHost(info);
			_hostptofd[hostkey] = host->getSocket();
			_servers[host->getSocket()] = host;
			addEvent(host->getSocket(),
					 EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR);
			_nbServ++;
		}
		freeaddrinfo(info);

		const int	 fd = _hostptofd[hostkey];
		const string server_name = findParam("server_name", "", *it);
		_servers[fd]->AddServer(server_name, *it);
		++it;
	}
}

void Handler::handleNewClient(const ServerHost *server, int client_socket,
							  sockaddr_storage *client_addr,
							  const string	   &request)
{
	const string server_name = client::Parser::findHostName(request);

	const VirtualServer *vhost = server->getVhost(server_name);
	if (vhost == NULL) {
		vhost = server->getDefaultVhost();
	}
	CSERVER = vhost;

	client::Client *client =
		new client::Client(server, vhost, client_socket, client_addr, _envp);
	_clients[client_socket] = client;

	client->setRequest(request);
	handleClientRequest(client_socket, request);
}

void Handler::handleNewConnection(const ServerHost *server)
{
	sockaddr_storage *client_addr = NULL;
	int				  client_socket = -1;
	string			  request;

	try {
		client_addr = new sockaddr_storage;
		memset(client_addr, 0, sizeof(sockaddr_storage));

		client_socket = server->acceptClient(client_addr);

		addEvent(client_socket, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR);
		request = ServerHost::recvRequest(client_socket);
	} catch (RecvException &e) {

		removeEvent(client_socket);
		LOG_ERROR(e.what());
		delete client_addr;
		if (client_socket != -1) {
			close(client_socket);
		}

	} catch (const std::exception &e) {

		LOG_ERROR(e.what());
		delete client_addr;
		if (client_socket != -1) {
			close(client_socket);
		}
	}

	if (request.empty()) {
		handleClientDisconnection(client_socket);
		return;
	}
	try {
		handleNewClient(server, client_socket, client_addr, request);
	} catch (const std::exception &e) {
		LOG_ERROR(e.what());
		handleClientDisconnection(client_socket);
	}
}

void Handler::handleClientRequest(int event_fd, const string &request)
{
	string client_request;

	Client *client = _clients[event_fd];
	if (!client) {
		return;
	}

	try {
		CSERVER = client->getServer();

		if (request.empty()) {
			client_request = ServerHost::recvRequest(event_fd);
			client->setRequest(client_request);

			const string host = client::Parser::findHostName(client_request);
			const ServerHost	*serverHost = client->getServerHost();
			const VirtualServer *vhost = serverHost->getVhost(host);
			if (vhost == NULL) {
				vhost = serverHost->getDefaultVhost();
			}
			client->setVHost(vhost);
		}
		client->handleRequest();

	} catch (const std::exception &e) {

		const Parser parser;
		Builder		*builder = new Builder(client->getServer(), parser);

		LOG_ERROR(e.what());
		builder->setCode("500");
		builder->findErrorPage();
		client->setBuilder(builder);
	}

	try {
		modifyEvent(event_fd, EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR);
	} catch (const std::exception &e) {
		LOG_ERROR(e.what());
		handleClientDisconnection(event_fd);
	}
}

void Handler::handleClientResponse(int event_fd)
{
	Client *client = _clients[event_fd];
	if (!client || (client && client->handleResponse() != FINISH)) {
		return;
	}

	try {
		CSERVER = client->getServer();
		ServerHost::sendResponse(event_fd, client->getResponse());
		modifyEvent(event_fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR);

	} catch (const std::exception &e) {
		LOG_ERROR(e.what());
		handleClientDisconnection(event_fd);
	}
}

void Handler::handleClientDisconnection(int event_fd)
{
	Client *client = _clients[event_fd];
	if (!client) {
		return;
	}

	CSERVER = client->getServer();

	_clients.erase(event_fd);
	try {
		removeEvent(event_fd);
	} catch (const std::exception &e) {
		LOG_ERROR(e.what());
	}
	close(event_fd);
	delete client;
}

void Handler::addEvent(int fd, uint32_t events) const
{
	struct epoll_event event = {};
	event.events = events;
	event.data.fd = fd;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event) == -1) {
		throw InternalServerException("epoll_ctl", __LINE__, __FILE__,
									  std::string(strerror(errno)));
	}
}

void Handler::removeEvent(int fd) const
{
	if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		throw InternalServerException("epoll_ctl", __LINE__, __FILE__,
									  std::string(strerror(errno)));
	}
}

void Handler::modifyEvent(int fd, uint32_t events) const
{
	struct epoll_event event = {};
	event.events = events;
	event.data.fd = fd;
	if (epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &event) == -1) {
		throw InternalServerException("epoll_ctl", __LINE__, __FILE__,
									  std::string(strerror(errno)));
	}
}

void Handler::runEventLoop()
{
	struct epoll_event event[MAX_EVENTS];
	int				   event_fd = 0;

	while (!g_sig) {

		const int nfds = epoll_wait(_epfd, event, MAX_EVENTS, -1);
		if (nfds == -1 && !g_sig) {
			throw InternalServerException("epoll_wait", __LINE__, __FILE__,
										  std::string(strerror(errno)));
		}
		for (int i = 0; i < nfds; ++i) {

			CSERVER = NULL;
			event_fd = event[i].data.fd;
			ServerHost *server = _servers[event_fd];

			if (server) {
				try {
					handleNewConnection(server);
				} catch (const std::exception &e) {
					LOG_ERROR(e.what());
				}
			}
			else if (event[i].events & EPOLLRDHUP ||
					 event[i].events & EPOLLERR || event[i].events & EPOLLHUP) {
				handleClientDisconnection(event_fd);
			}
			else if (event[i].events & EPOLLIN) {
				handleClientRequest(event_fd);
			}
			else if (event[i].events & EPOLLOUT) {
				handleClientResponse(event_fd);
			}
		}
		if (Logger::Instance != NULL) {
			Logger::Instance->flush();
		}
	}
}

Handler::~Handler()
{
	std::map< int, ServerHost * >::iterator it = _servers.begin();
	while (it != _servers.end()) {
		delete it->second;
		++it;
	}
	std::map< int, Client * >::iterator it2 = _clients.begin();
	while (it2 != _clients.end()) {
		delete it2->second;
		++it2;
	}
	close(_epfd);
}