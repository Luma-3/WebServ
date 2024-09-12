/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:19:07 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/12 23:42:40 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Handler.hpp"

int main(void)
{
	// if (argc != 2)
	// 	return (1);

	Handler	serverVect = Handler();
	// The servers instanciation will happen in there. They will all be pushed 
	// back into a vector. The Config struct and its function will be called in there.
	// It probably will need to be initialized in main and therefore be passed as an
	// argument to the loadServ() method ?
	serverVect.loadServTest(); 
	
	// creation of epoll instance. it will be used to manage the sockets of all servers;
	int		epfd = epoll_create1(0);
	if (epfd == -1)
	{
		std::cerr << "Error on epoll_create" << std::endl;
		return (-1);
	}

	for (int i = 0; i < serverVect.getNbServ(); ++i)
	{
		// opening all servers sockets		
		serverVect[i]->createSocket();

		// creation of the struct epoll event that defines the events to watch
		struct epoll_event	event;
		// watch only reading events on the given socket
		event.events  = EPOLLIN;
		event.data.fd = serverVect[i]->getSocket();

		// adds the socket to the elements to watch
		// to modify a socket : EPOLL_CTL_MOD. to delete a socket : EPOLL_CTL_DEL
		if (epoll_ctl(epfd, EPOLL_CTL_ADD, serverVect[i]->getSocket(), &event) == -1)
		{
			std::cerr << "Error on epoll_ctl" << std::endl;
			return (1);
		}
	}
	struct epoll_event	events[MAX_EVENTS];
	while (42)
	{
		// wait for events to get. the last flag is the timeout, -1 being infinite
		int	nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		if (nfds == -1)
		{
			std::cout << "Error on epoll_wait" << std::endl;
			return (1);
		}
		for (int i = 0; i < nfds; ++i)
		{
			// if epoll detects an event : checks on which socket it happenend and calls on the method from the designated server
			if (events[i].events & EPOLLIN) {
				for (int j = 0; j < serverVect.getNbServ(); j++) {
					if (events[i].data.fd == serverVect[j]->getSocket()) {
						serverVect[j]->HandleConnexion();
						break;
					}
				}
			}
		}
	}
	return (0);
}