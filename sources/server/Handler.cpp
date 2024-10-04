/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/10/04 13:42:08 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Handler.hpp"

#include <algorithm>
#include <map>

#include "client/Builder.hpp"
#include "client/Parser.hpp"
#include "server/Server.hpp"
#include "server/ServerException.hpp"
#include "server/Signal.hpp"

volatile int g_sig = 0;

Handler::Handler() : _epfd(-1), _nbServ(0) {}

Handler::Handler(const std::vector< statement::Server * > &servers) :
	_epfd(epoll_create1(0)),
	_nbServ(0)
{
	if (_epfd == -1) {
		throw InternalServerException("Error on epoll_create");
	}

	std::vector< statement::Server * >::const_iterator it = servers.begin();

	while (it != servers.end()) {
		Server *server = new Server(*it);
		addEvent(server->getSocket(), EPOLLIN | EPOLLRDHUP);
		_servers.push_back(server);
		++it;
	}
}

Handler::Handler(const Handler &src) :
	_epfd(epoll_create1(0)),
	_nbServ(src._nbServ)
{
	if (_epfd == -1) {
		throw InternalServerException("Error on epoll_create");
	}

	std::vector< Server * >::const_iterator it = src._servers.begin();

	while (it != _servers.end()) {
		_servers.push_back(new Server(**it));
		++it;
	}
}

Handler &Handler::operator=(const Handler &rhs)
{
	if (this == &rhs) {
		return (*this);
	}
	_nbServ = rhs._nbServ;
	std::vector< Server * >::const_iterator it = rhs._servers.begin();

	while (it != _servers.end()) {
		_servers.push_back(new Server(**it));
		++it;
	}
	return (*this);
}

Server *Handler::findServer(int fd)
{
	for (std::vector< Server * >::const_iterator it = _servers.begin();
		 it < _servers.end(); ++it) {
		if (fd == (*it)->getSocket()) {
			return (*it);
		}
	}
	return (NULL);
}

client::Client *Handler::findClient(int fd)
{
	for (std::vector< client::Client * >::iterator it = _clients.begin();
		 it < _clients.end(); ++it) {
		if (fd == (*it)->getSocket()) {
			return (*it);
		}
	}
	return (NULL);
}

void Handler::handleNewConnection(const Server *server)
{
	int client_socket = server->acceptRequest();

	client::Client *client = new client::Client(server, NULL, client_socket);
	addEvent(client_socket, EPOLLIN | EPOLLRDHUP);
	client->receiveRequest();
	client->handleRequest();
	// modifyEvent(client_socket, EPOLLOUT | EPOLLRDHUP);
	client->sendResponse();
	// modifyEvent(client_socket, EPOLLIN | EPOLLRDHUP);
	_clients.push_back(client);
}

void Handler::handleClientRequest(int event_fd)
{
	client::Client *client = findClient(event_fd);
	if (client) {
		client->receiveRequest();
		client->handleRequest();
		// modifyEvent(event_fd, EPOLLOUT | EPOLLRDHUP);
		client->sendResponse();
		// modifyEvent(event_fd, EPOLLIN | EPOLLRDHUP);
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
		throw InternalServerException("Error on epoll_ctl");
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

int Handler::runEventLoop()
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
			Server *server = findServer(event_fd);
			if (server) {
				handleNewConnection(server);
			}
			else if (event[i].events & EPOLLRDHUP) {
				// TODO : handle client disconnection on a disociated Method
				client::Client *client = findClient(event_fd);
				if (client) {
					_clients.erase(
						std::remove(_clients.begin(), _clients.end(), client),
						_clients.end());
					close(event_fd);
				}
			}
			else {
				handleClientRequest(event_fd);
			}
		}
	}
	return (SUCCESS);
}

Handler::~Handler()
{
	std::vector< Server * >::iterator it = _servers.begin();

	while (it != _servers.end()) {
		delete *it;
		++it;
	}
}