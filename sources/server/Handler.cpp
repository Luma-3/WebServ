/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/10/08 17:59:14 by jdufour          ###   ########.fr       */
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

Handler::Handler() : _epfd(-1) {}

Handler::Handler(const std::vector< statement::Server * > &servers) : _epfd(-1)
{
	std::vector< statement::Server * >::const_iterator it = servers.begin();

	while (it != servers.end()) {
		_servers.push_back(new Server(*it));
		++it;
	}
}

Handler::Handler(const Handler &src) : _epfd(src._epfd)
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

void Handler::loadServTest()
{
	std::string names[4] = {"server1", "server2", "server3", "server4"};
	std::string ports[4] = {"8080", "8081", "8082", "9092"};

	for (int i = 0; i < 4; i++) {
		std::string name = names[i];
		std::string port = ports[i];
		_servers.push_back(new Server(name, "localhost", port));
	}
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
	struct epoll_event events[MAX_EVENTS];
	char			   tmp[566] =
		"HTTP/1.1 200 OK\r\n"
		"Date: Wed, 27 Sep 2023 12:00:00 GMT\r\n"
		"Content-Length: 446\r\n"
		"Connection: keep-alive\r\n"
		"\r\n"
		"<!DOCTYPE html>"
		"<html lang='en'>"
		"<head>"
		"<meta charset='UTF-8'>"
		"<meta name='viewport' content='width=device-width, "
		"initial-scale=1.0'>"
		"<title>Test CSS</title>"
		"<style>"
		"body { font-family: Arial, sans-serif; text-align: center; }"
		"h1 { color: #4CAF50; }"
		"p { color: #333; font-size: 14px; }"
		"div { margin: 20px auto; width: 50%; padding: 10px; border: "
		"1px "
		"solid #4CAF50; }"
		"</style>"
		"</head>"
		"<body>"
		"<div>"
		"<h1>Welcome</h1>"
		"<p>This is a simple page with CSS styling.</p>"
		// "<img src=\"/image.jpg\" alt=\"Sample Image\" width=\"200\">"
		"</div>"
		"</body>"
		"</html>";
	std::string repTest(tmp);

	while (!g_sig) {
		initSignal();
		int nfds = epoll_wait(_epfd, events, MAX_EVENTS, -1);
		if (nfds == -1 && !g_sig) {
			std::cout << "Error on epoll_wait" << std::endl;
			return (FAILURE);
		}
		for (int i = 0; i < nfds; i++) {
			int								  event_fd = events[i].data.fd;
			std::vector< Server * >::iterator it;

			for (it = _servers.begin(); it < _servers.end(); it++) {
				if (event_fd == (*it)->getSocket()) {
					(*it)->acceptRequest(_epfd);
				}
			}
			if (it == _servers.end()) {
				for (it = _servers.begin(); it < _servers.end(); it++) {
					std::vector< int >::const_iterator it2 =
						(*it)->getClientSock().begin();
					for (; it2 < (*it)->getClientSock().end(); it2++) {
						if (event_fd == (*it2)) {
							int	req_nb = std::distance((*it)->getClientSock().begin(), it2);
							if (req_nb >= 0)
							{
								(*it)->receiveRequest(_epfd, *it2, req_nb);
								(*it)->epollmod(_epfd, *it2, EPOLLOUT);
								(*it)->sendResponse(repTest, *it2);
								(*it)->epollmod(_epfd, *it2, EPOLLIN);
							}
						}
					}
				}
			}
		}
	}
	return (SUCCESS);
}

// int Handler::handleEvents()
// {
// 	struct epoll_event event[MAX_EVENTS];

// 	while (!g_sig) {
// 		initSignal();
// 		int nfds = epoll_wait(_epfd, event, MAX_EVENTS, -1);
// 		if (nfds == -1 && !g_sig) {
// 			throw InternalServerException("Error on epoll_wait");
// 		}
// 		for (int i = 0; i < nfds; ++i) {
// 			int event_fd = event[i].data.fd;

// 			for (std::vector< Server * >::iterator it = _servers.begin();
// 				 it < _servers.end(); ++it) {
// 				if (event_fd == (*it)->getSocket()) {
// 					(*it)->acceptRequest(_epfd);
// 					// (*it)->receiveRequest(_epfd);
// 					// std::cout << "Request received" << (*it)->getRequest()
// 					// 		  << std::endl;
// 					// client::Client client(**it);
// 					// client::Parser parser;
// 					// parser.parseRequest((*it)->getRequest());
// 					// client.setParser(parser);
// 					// (*it)->sendResponse(client.buildResponse());
// 				}
// 				for (std::vector< int >::iterator it2 = sock_tab.begin();
// 					 it2 < sock_tab.end(); ++it) {
// 					if (event_fd == *it2) {
// 						(*it)->receiveRequest(_epfd);
// 						client::Client client(**it);
// 						client::Parser parser;
// 						parser.parseRequest((*it)->getRequest());
// 						client.setParser(parser);
// 						(*it)->epollmod(_epfd, (*it)->getClientSock()[i],
// 										EPOLLOUT);
// 						(*it)->sendResponse(client.buildResponse());
// 						(*it)->epollmod(_epfd, (*it)->getClientSock()[i],
// 										EPOLLIN | EPOLLET);
// 					}
// 					break;
// 				}
// 			}
// 		}
// 	}
// 	return (SUCCESS);
// }

Handler::~Handler()
{
	std::vector< Server * >::iterator it = _servers.begin();

	while (it != _servers.end()) {
		delete *it;
		++it;
	}
}