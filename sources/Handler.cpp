/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/09/18 22:49:39 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Handler.hpp"

#include "../includes/Signal.hpp"

volatile int g_sig = 0;

Handler::Handler() : _nbServ(0) {}

// void	Handler::loadServ()
// {
// 	Config	serverConf = Config.getConf(); //to be replaced by the real function
// 	_nbServ =  serverConf.getNbServers(); //idem

// 	for (int i = 0; i < _nbServ; i++)
// 	{
// 		// these 3 will be rewritten when the config class will be set
// 		std::string	name = serverConf.getServerName(i);
// 		std::string	hostname = serverConf.getHostname(i);
// 		std::string	port = serverConf.getPort(i);
// 		Server	server = Server( name, hostname, port);
// 		_servers.push_back(server);
// 	}
// }

Handler::Handler(const Handler &src)
{
	*this = src;
}

Handler &Handler::operator=(const Handler &rhs)
{
	this->_epfd = epoll_create1(0);
	this->_nbServ = rhs._nbServ;
	this->_epfd = rhs._epfd;
	for (int i = 0; i < _nbServ; i++) {
		_servers.push_back(new Server(rhs._servers[i]->getName(),
									  rhs._servers[i]->getHost(),
									  rhs._servers[i]->getPort()));
	}
	return (*this);
}

void Handler::loadServTest()
{
	_nbServ = 4;
	std::string names[4] = {"server1", "server2", "server3", "server4"};
	std::string ports[4] = {"8080", "8081", "8082", "9092"};

	for (int i = 0; i < _nbServ; i++) {
		std::string name = names[i];
		std::string port = ports[i];
		_servers.push_back(new Server(name, "localhost", port));
	}
}

int Handler::launchServers()
{
	_epfd = epoll_create1(0);
	if (_epfd == -1) {
		std::cerr << "Error on epoll_create" << std::endl;
		return (FAILURE);
	}

	for (std::vector<Server *>::iterator it = _servers.begin(); it < _servers.end(); ++it) {
		(*it)->createSocket();
		(*it)->setSocket();
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = (*it)->getSocket();
		if (epoll_ctl(_epfd, EPOLL_CTL_ADD, (*it)->getSocket(), &event) ==
			-1) {
			std::cerr << "Error on epoll_ctl" << std::endl;
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

int Handler::handleEvents()
{
	struct epoll_event events[MAX_EVENTS];

	while (!g_sig) {
		initSignal();
		int nfds = epoll_wait(_epfd, events, MAX_EVENTS, -1);
		if (nfds == -1 && !g_sig) {
			std::cout << "Error on epoll_wait" << std::endl;
			return (FAILURE);
		}
		for (int i = 0; i < nfds; ++i) {
			if (events[i].events & EPOLLIN) {
				for (std::vector<Server *>::iterator it = _servers.begin();
					 it < _servers.end(); ++it) {
					if (events[i].data.fd == (*it)->getSocket()) {
						(*it)->HandleConnexion();
						break;
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
// 		throw(std::out_of_range("Error : invalid access index on Servers"));
// 	return (_servers[index]);
// }

Handler::~Handler()
{
	for (int i = 0; i < _nbServ; ++i)
		delete _servers[i];
}