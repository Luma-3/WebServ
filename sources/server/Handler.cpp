/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/10/30 14:32:11 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Handler.hpp"

#include <algorithm>
#include <map>
#include <sys/socket.h>

#include "client/Builder.hpp"
#include "client/Parser.hpp"
#include "server/ServerException.hpp"
#include "server/ServerHost.hpp"

using std::string;

volatile int g_sig = 0;

Handler::Handler(const std::vector< VirtualServer * > &servers) :
	_epfd(epoll_create1(0)),
	_nbServ(0)
{
	if (_epfd == -1) {
		throw InternalServerException("epoll_create", __LINE__, __FILE__,
									  std::string(strerror(errno)));
	}

	std::vector< VirtualServer * >::const_iterator it = servers.begin();

	while (it != servers.end()) {
		const Param *listen = (*it)->getParam("listen");

		std::string hostkey =
			listen->getPair().first + ":" + listen->getPair().second;

		if (_hostptofd.find(hostkey) == _hostptofd.end()) {

			ServerHost *host = new ServerHost(listen->getPair().first,
											  listen->getPair().second);
			_hostptofd[hostkey] = host->getSocket();
			_servers[host->getSocket()] = host;
			addEvent(host->getSocket(), EPOLLIN);
			_nbServ++;
		}
		int fd = _hostptofd[hostkey];
		_servers[fd]->AddServer((*it)->getParam("hostname")->getValue(), *it);
		++it;
	}
}

void Handler::handleNewClient(const ServerHost *server, int client_socket,
							  sockaddr_storage	*client_addr,
							  const std::string &request)
{
	string hostname = client::Parser::findHostName(request);

	const VirtualServer *vhost = server->getVhost(hostname);
	if (vhost == NULL) {
		vhost = server->getDefaultVhost();
	}
	_CSERVER = vhost;

	client::Client *client = new client::Client(
		vhost, server->getDefaultVhost(), client_socket, client_addr);
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
		client_socket = server->acceptClient(client_addr);
		addEvent(client_socket, EPOLLIN | EPOLLRDHUP);
		request = ServerHost::recvRequest(client_socket);
	} catch (RecvException &e) {
		removeEvent(client_socket);
		LOG_ERROR(e.what(), _CSERVER);
		if (client_addr) {
			delete client_addr;
		}
		if (client_socket != -1) {
			close(client_socket);
		}
	} catch (const std::exception &e) {
		LOG_ERROR(e.what(), _CSERVER);
		if (client_addr) {
			delete client_addr;
		}
		if (client_socket != -1) {
			close(client_socket);
		}
	}
	try {
		handleNewClient(server, client_socket, client_addr, request);
	} catch (const std::exception &e) {
		LOG_ERROR(e.what(), _CSERVER);
		handleClientDisconnection(
			client_socket); // TODO : method for error disconnection
	}
}

void Handler::handleClientRequest(int event_fd, const std::string &request)
{
	std::string client_request;

	client::Client *client = _clients[event_fd];
	if (client) {
		try {
			if (request.empty()) {
				_CSERVER = client->getServer();
				client_request = ServerHost::recvRequest(event_fd);
				client->setRequest(client_request);
			}
			client->handleRequest();
		} catch (const std::exception &e) {
			LOG_ERROR(e.what(), _CSERVER);
			client->setResponse(e.what());
		}
		try {
			modifyEvent(event_fd, EPOLLOUT | EPOLLRDHUP);
		} catch (const std::exception &e) {
			LOG_ERROR(e.what(), _CSERVER);
			handleClientDisconnection(
				event_fd); // TODO : method for error disconnection
		}
	}
}

void Handler::handleClientResponse(int event_fd)
{
	client::Client *client = _clients[event_fd];
	if (client && client->handleResponse() == FINISH) {
		try {
			_CSERVER = client->getServer();
			ServerHost::sendResponse(event_fd, client->getResponse());
			modifyEvent(event_fd, EPOLLIN | EPOLLRDHUP);
		} catch (const std::exception &e) {
			LOG_ERROR(e.what(), _CSERVER);
			handleClientDisconnection(
				event_fd); // TODO : method for error disconnection
		}
	}
}

void Handler::handleClientDisconnection(int event_fd)
{
	client::Client *client = _clients[event_fd];
	if (client) {
		_CSERVER = client->getServer();
		_clients.erase(event_fd);
		try {
			removeEvent(event_fd);
		} catch (const std::exception &e) {
			LOG_ERROR(e.what(), _CSERVER);
		}
		close(event_fd);
		delete client;
	}
}

void Handler::addEvent(int fd, uint32_t events) const
{
	struct epoll_event event;
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
	struct epoll_event event;
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
	int				   event_fd;

	LOG_DEBUG("Starting event loop", NULL);
	while (!g_sig) {
		int nfds = epoll_wait(_epfd, event, MAX_EVENTS, -1);
		if (nfds == -1 && !g_sig) {
			throw InternalServerException("epoll_wait", __LINE__, __FILE__,
										  std::string(strerror(errno)));
		}
		for (int i = 0; i < nfds; ++i) {
			_CSERVER = NULL;
			event_fd = event[i].data.fd;
			ServerHost *server = _servers[event_fd];
			if (server) {
				try {
					handleNewConnection(server);
				} catch (const std::exception &e) {
					LOG_ERROR(e.what(), _CSERVER);
				}
			}
			else if (event[i].events & EPOLLRDHUP) {
				handleClientDisconnection(event_fd);
			}
			else if (event[i].events & EPOLLIN) {
				handleClientRequest(event_fd);
			}
			else if (event[i].events & EPOLLOUT) {
				handleClientResponse(event_fd);
			} // TODO : method for error disconnection
		}
		Logger::Instance->flush();
	}
}

Handler::~Handler()
{
	std::map< int, ServerHost * >::iterator it = _servers.begin();
	while (it != _servers.end()) {
		delete it->second;
		++it;
	}
	close(_epfd);
}