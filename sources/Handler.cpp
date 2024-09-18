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

Handler::Handler() {}

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
	// creation of epoll instance. it will be used to manage the sockets of all
	// servers;
	int epfd = epoll_create1(0);
	if (epfd == -1) {
		std::cerr << "Error on epoll_create" << std::endl;
		return (FAILURE);
	}

	for (int i = 0; i < _nbServ; ++i) {
		// opening all servers sockets
		_servers[i]->createSocket();

		// creation of the struct epoll event that defines the events to watch
		struct epoll_event event;
		// watch only reading events on the given socket
		event.events = EPOLLIN;
		event.data.fd = _servers[i]->getSocket();

		// adds the socket to the elements to watch
		// to modify a socket : EPOLL_CTL_MOD. to delete a socket :
		// EPOLL_CTL_DEL
		if (epoll_ctl(epfd, EPOLL_CTL_ADD, _servers[i]->getSocket(), &event) ==
			-1) {
			std::cerr << "Error on epoll_ctl" << std::endl;
			return (FAILURE);
		}
	}
	struct epoll_event events[MAX_EVENTS];
	while (!g_sig) {
		initSignal();
		// wait for events to get. the last flag is the timeout, -1 being
		// infinite

		int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		if (nfds == -1 && !g_sig) {
			std::cout << "Error on epoll_wait" << std::endl;
			return (FAILURE);
		}
		for (int i = 0; i < nfds; ++i) {
			// if epoll detects an event : checks on which socket it happenend
			// and calls on the method from the designated server
			if (events[i].events & EPOLLIN) {
				for (int j = 0; j < _nbServ; j++) {
					if (events[i].data.fd == _servers[j]->getSocket()) {
						_servers[j]->HandleConnexion();
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

std::vector<Server *> Handler::getAllServ(void) const
{
	return (_servers);
}

int Handler::getNbServ(void) const
{
	return (_nbServ);
}

Handler::~Handler()
{
	for (int i = 0; i < _nbServ; ++i)
		delete _servers[i];
}