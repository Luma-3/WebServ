/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/10/03 15:45:36 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Handler.hpp"

#include <algorithm>
#include <map>

#include "client/Builder.hpp"
#include "client/Client.hpp"
#include "client/Parser.hpp"
#include "server/ServerException.hpp"
#include "server/Signal.hpp"

volatile int g_sig = 0;

Handler::Handler() : _nbServ(0), _epfd(-1) {}

Handler::Handler(const std::vector< statement::Server * > &servers) :
	_nbServ(0),
	_epfd(-1)
{
	std::vector< statement::Server * >::const_iterator it = servers.begin();

	while (it != servers.end()) {
		_servers.push_back(new Server(*it));
		++it;
	}
}

Handler::Handler(const Handler &src) : _nbServ(src._nbServ), _epfd(src._epfd)
{
	*this = src;
}

Handler &Handler::operator=(const Handler &rhs)
{
	if (this == &rhs) {
		return (*this);
	}
	this->_epfd = epoll_create1(0);
	this->_epfd = rhs._epfd;
	std::vector< Server * >::const_iterator it = rhs._servers.begin();

	while (it != _servers.end()) {
		_servers.push_back(new Server(**it));
		++it;
	}
	return (*this);
}

int Handler::launchServers()
{

	// deplacer epoll_create dans le constructeur
	_epfd = epoll_create1(0);
	if (_epfd == -1) {
		throw InternalServerException("Error on epoll_create");
	}

	for (std::vector< Server * >::iterator it = _servers.begin();
		 it < _servers.end(); ++it) {
		(*it)->createSocket();
		(*it)->setSocket();
		(*it)->epolladd(_epfd, (*it)->getSocket());
	}

	return (SUCCESS);
}

Server *findServer(int fd, const std::vector< Server * > &_servers)
{
	for (std::vector< Server * >::const_iterator it = _servers.begin();
		 it < _servers.end(); ++it) {
		if (fd == (*it)->getSocket()) {
			return (*it);
		}
	}
	return (NULL);
}

client::Client *findClient(int fd, std::vector< client::Client * > &clients)
{
	for (std::vector< client::Client * >::iterator it = clients.begin();
		 it < clients.end(); ++it) {
		if (fd == (*it)->getSocket()) {
			return (*it);
		}
	}
	return (NULL);
}

int Handler::handleEvents()
{
	struct epoll_event event[MAX_EVENTS];
	// std::vector< int > sock_tab;

	client::Builder builder;

	std::vector< client::Client * > clients;

	while (!g_sig) {
		std::cout << "TOUR DE BOUCLE" << std::endl;
		int nfds = epoll_wait(_epfd, event, MAX_EVENTS, -1);
		if (nfds == -1 && !g_sig) {
			throw InternalServerException("Error on epoll_wait");
		}
		for (int i = 0; i < nfds; ++i) {
			int event_fd = event[i].data.fd;
			// denest this loop Add functuion findServer with fd !
			// think about deconection event with flag EPOLLRDHUP
			// for (std::vector< Server * >::iterator it = _servers.begin();
			// 	 it < _servers.end(); ++it) {
			// 	if (event_fd == (*it)->getSocket()) {
			// 		(*it)->acceptRequest(_epfd, sock_tab);
			// 		// (*it)->receiveRequest(_epfd);
			// 		// std::cout << "Request received" << (*it)->getRequest()
			// 		// 		  << std::endl;
			// 		// client::Client client(**it);
			// 		// client::Parser parser;
			// 		// parser.parseRequest((*it)->getRequest());
			// 		// client.setParser(parser);
			// 		// (*it)->sendResponse(client.buildResponse());
			// 	}
			// 	// create fonction findClient with fd
			// 	for (std::vector< int >::iterator it2 = sock_tab.begin();
			// 		 it2 < sock_tab.end(); ++it) {
			// 		if (event_fd == *it2) {
			// 			std::cout << "ICI" << std::endl;
			// 			(*it)->receiveRequest(_epfd);
			// 			client::Client client(**it);
			// 			client::Parser parser;
			// 			parser.parseRequest((*it)->getRequest());
			// 			client.setParser(parser);
			// 			(*it)->epollmod(_epfd, (*it)->getNewSocket(), EPOLLOUT);
			// 			(*it)->sendResponse(client.buildResponse());
			// 			(*it)->epollmod(_epfd, (*it)->getNewSocket(),
			// 							EPOLLIN | EPOLLET);
			// 		}
			// 		break;
			// 	}
			// }

			Server *server = findServer(event_fd, _servers);
			std::cout << "event_fd : " << event_fd << std::endl;

			if (server) {
				int client_socket = server->acceptRequest(_epfd);

				client::Client *client =
					new client::Client(server, NULL, client_socket);
				client->receiveRequest();
				client->handleRequest();
				client->sendResponse();
				clients.push_back(client);
			}
			else if (event[i].events & EPOLLRDHUP) {
				client::Client *client = findClient(event_fd, clients);
				if (client) {
					clients.erase(
						std::remove(clients.begin(), clients.end(), client),
						clients.end());
					close(event_fd);
				}
			}
			else {
				client::Client *client = findClient(event_fd, clients);
				if (client) {
					client->receiveRequest();
					client->handleRequest();
					client->sendResponse();
				}
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