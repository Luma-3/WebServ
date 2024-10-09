/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:46:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 14:32:07 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
#define HANDLER_HPP

#define MAX_EVENTS 10

#include <csignal>
#include <exception>
#include <sys/epoll.h>
#include <vector>

#include "client/Client.hpp"
#include "lexer/Token.hpp"
#include "Server.hpp"

class Handler
{
  private:
	const int						_epfd;
	int								_nbServ;
	std::vector< Server * >			_servers;
	std::vector< client::Client * > _clients;

	Server		   *findServer(int fd);
	client::Client *findClient(int fd);

	void handleNewConnection(const Server *server);
	void handleClientRequest(int event_fd);

	void		  addEvent(int fd, uint32_t events) const;
	void		  removeEvent(int fd) const;
	void		  modifyEvent(int fd, uint32_t events) const;
	const Server *getDefaultServer(const std::string &port,
								   const std::string &host) const;

  public:
	Handler();
	Handler(const std::vector< statement::Server * > &servers);
	Handler(const Handler &src);
	Handler &operator=(const Handler &rhs);

	int runEventLoop();

	~Handler();
};

#endif