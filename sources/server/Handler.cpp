/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/09/25 11:14:46 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Handler.hpp"

#include <algorithm>
#include <map>

#include "client/Client.hpp"
#include "client/Client_Parser.hpp"
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

int Handler::handleEvents()
{
	struct epoll_event event[MAX_EVENTS];
	std::vector< int > sock_tab;

	while (!g_sig) {
		initSignal();
		int nfds = epoll_wait(_epfd, event, MAX_EVENTS, -1);
		if (nfds == -1 && !g_sig) {
			throw InternalServerException("Error on epoll_wait");
		}
		for (int i = 0; i < nfds; ++i) {
			int event_fd = event[i].data.fd;

			for (std::vector< Server * >::iterator it = _servers.begin();
				 it < _servers.end(); ++it) {
				if (event_fd == (*it)->getSocket()) {
					(*it)->acceptRequest(_epfd, sock_tab);
					// (*it)->receiveRequest(_epfd);
					// std::cout << "Request received" << (*it)->getRequest()
					// 		  << std::endl;
					// client::Client client(**it);
					// client::Parser parser;
					// parser.parseRequest((*it)->getRequest());
					// client.setParser(parser);
					// (*it)->sendResponse(client.buildResponse());
				}
				for (std::vector< int >::iterator it2 = sock_tab.begin();
					 it2 < sock_tab.end(); ++it) {
					if (event_fd == *it2) {
						std::cout << "ICI" << std::endl;
						(*it)->receiveRequest(_epfd);
						client::Client client(**it);
						client::Parser parser;
						parser.parseRequest((*it)->getRequest());
						client.setParser(parser);
						(*it)->epollmod(_epfd, (*it)->getNewSocket(), EPOLLOUT);
						(*it)->sendResponse(client.buildResponse());
						(*it)->epollmod(_epfd, (*it)->getNewSocket(),
										EPOLLIN | EPOLLET);
					}
					break;
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