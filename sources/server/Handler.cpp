/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/09/25 00:59:08 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Handler.hpp"

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
		struct epoll_event event = {.events = EPOLLIN,
									.data = {.fd = (*it)->getSocket()}};
		if (epoll_ctl(_epfd, EPOLL_CTL_ADD, (*it)->getSocket(), &event) == -1) {
			throw InternalServerException("Error on epoll_ctl");
		}
	}
	return (SUCCESS);
}

// if (events[i].events & EPOLLIN) {
int Handler::handleEvents()
{
	struct epoll_event events[MAX_EVENTS];

	while (!g_sig) {

		initSignal();
		std::cout << "Je repart pour un tour de boucle " << std::endl;

		int nfds = epoll_wait(_epfd, events, MAX_EVENTS, -1);
		if (nfds == -1 && !g_sig) {
			throw InternalServerException("Error on epoll_wait");
		}
		for (int i = 0; i < nfds; ++i) {
			int event_fd = events[i].data.fd;

			for (std::vector< Server * >::iterator it = _servers.begin();
				 it < _servers.end(); ++it) {
				if (event_fd == (*it)->getSocket()) {

					(*it)->receiveRequest(_epfd);
					int new_socket = (*it)->getNewSocket();
					_client_socket.push_back(new_socket);

					std::cout << "Request received" << (*it)->getRequest()
							  << std::endl;
					client::Client client(**it);
					client::Parser parser;
					parser.parseRequest((*it)->getRequest());
					client.setParser(parser);
					(*it)->sendResponse(client.buildResponse(), new_socket);
				}
				for (std::vector< int >::iterator iterator =
						 _client_socket.begin();
					 iterator != _client_socket.end(); ++iterator) {
					if (event_fd == *iterator) {
						std::string request =
							(*it)->receiveClientRequest(event_fd);
						client::Client client(**it);
						client::Parser parser;
						parser.parseRequest(request);
						client.setParser(parser);
						(*it)->sendResponse(client.buildResponse(), event_fd);
						struct epoll_event ev;
						ev.events = EPOLLIN | EPOLLET;
						ev.data.fd = event_fd;
						epoll_ctl(_epfd, EPOLL_CTL_MOD, event_fd, &ev);
					}
				}
			}
		}
	}
	return (SUCCESS);
}
// Server *Handler::operator[](const int index)
// {
// 	if (index < 0 || index >= _nbServ || !_nbServ)
// 		throw(std::out_of_range("Error : invalid access index on
// Servers")); 	return (_servers[index]);
// }

Handler::~Handler()
{
	std::vector< Server * >::iterator it = _servers.begin();

	while (it != _servers.end()) {
		delete *it;
		++it;
	}
}