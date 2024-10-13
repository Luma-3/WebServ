/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/10/13 12:56:46 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Handler.hpp"

#include <algorithm>
#include <map>

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
		throw InternalServerException("Error on epoll_create");
	}

	std::cout << "Handler created" << std::endl;

	std::vector< VirtualServer * >::const_iterator it = servers.begin();

	while (it != servers.end()) {
		const Param *listen = (*it)->getParam("listen");

		std::string hostkey =
			listen->getPair().first + ":" + listen->getPair().second;
		std::cout << "HostKey: " << hostkey << std::endl;

		if (_hostptofd.find(hostkey) == _hostptofd.end()) {
			std::cout << "ServerHost not found, creating one" << std::endl;

			ServerHost *host = new ServerHost(listen->getPair().first,
											  listen->getPair().second);
			_hostptofd[hostkey] = host->getSocket();
			_servers[host->getSocket()] = host;
			addEvent(host->getSocket(), EPOLLIN | EPOLLRDHUP);
			_nbServ++;
		}
		int fd = _hostptofd[hostkey];
		std::cout << "Adding server to ServerHost" << std::endl;
		_servers[fd]->AddServer((*it)->getParam("hostname")->getValue(), *it);
		++it;
	}
}

void Handler::handleNewConnection(const ServerHost *server)
{
	int client_socket = server->acceptClient();

	string request = server->recvRequest(client_socket);
	string hostname = findHostName(request);

	const VirtualServer *vhost = server->getVhost(hostname);
	if (vhost == NULL) {
		vhost = server->getDefaultVhost();
	}

	client::Client *client =
		new client::Client(vhost, server->getDefaultVhost(), client_socket);
	client->setRequest(request);

	addEvent(client_socket, EPOLLIN | EPOLLRDHUP);
	_clients[client_socket] = client;
}

void Handler::handleClientRequest(int event_fd)
{
	client::Client *client = _clients[event_fd];
	if (client) {
		if (client->getRequest().find("\r\n\r\n") != std::string::npos) {
			client->handleRequest();
			modifyEvent(event_fd, EPOLLOUT | EPOLLRDHUP);
			std::cout << "Client request received" << client->getRequest()
					  << std::endl;
		}
	}
}

void Handler::handleClientResponse(int event_fd)
{
	client::Client *client = _clients[event_fd];
	if (client) {

		modifyEvent(event_fd, EPOLLIN | EPOLLRDHUP);
	}
}

void Handler::addEvent(int fd, uint32_t events) const
{
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event) == -1) {
		throw InternalServerException("Error on epoll_ctl");
	}
}

void Handler::removeEvent(int fd) const
{
	if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		throw InternalServerException("Error on epoll_ctl Remove");
	}
}

void Handler::modifyEvent(int fd, uint32_t events) const
{
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;
	if (epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &event) == -1) {
		throw InternalServerException("Error on epoll_ctl");
	}
}

void Handler::runEventLoop()
{
	struct epoll_event event[MAX_EVENTS];
	int				   event_fd;

	client::Builder builder;

	while (!g_sig) {
		std::cout << "Waiting for events" << std::endl;
		int nfds = epoll_wait(_epfd, event, MAX_EVENTS, -1);
		std::cout << "Events received" << std::endl;
		if (nfds == -1 && !g_sig) {
			throw InternalServerException("Error on epoll_wait");
		}
		for (int i = 0; i < nfds; ++i) {
			event_fd = event[i].data.fd;
			ServerHost *server = _servers[event_fd];
			if (server) {
				handleNewConnection(server);
				continue;
			}
			else if (event[i].events & EPOLLRDHUP) {
				// TODO : handle client disconnection on a disociated
				// Method
				client::Client *client = _clients[event_fd];
				std::cout << "Client disconnected" << std::endl;
				if (client) {
					_clients.erase(event_fd);
					removeEvent(event_fd);
					close(event_fd);
					delete client;
				}
			}
			else if (event[i].events & EPOLLIN) {
				handleClientRequest(event_fd);
				continue;
			}
			else if (event[i].events & EPOLLOUT) {
				handleClientResponse(event_fd);
				continue;
			}
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
	close(_epfd);
}