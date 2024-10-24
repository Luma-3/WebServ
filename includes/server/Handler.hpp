/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:46:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/24 14:58:11 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
#define HANDLER_HPP

#define MAX_EVENTS 10

#include <csignal>
#include <exception>
#include <map>
#include <sys/epoll.h>
#include <vector>

#include "client/Client.hpp"
#include "lexer/Token.hpp"
#include "VirtualServer.hpp"

class Handler
{
  private:
	const int						  _epfd;
	int								  _nbServ;
	std::map< std::string, int >	  _hostptofd;
	std::map< int, ServerHost * >	  _servers;
	std::map< int, client::Client * > _clients;

	void handleNewConnection(const ServerHost *server);
	void handleClientRequest(int event_fd);
	void handleClientResponse(int event_fd);

	int addEvent(int fd, uint32_t events) const;
	int removeEvent(int fd) const;
	int modifyEvent(int fd, uint32_t events) const;

  public:
	Handler(const std::vector< VirtualServer * > &servers);

	void runEventLoop();

	~Handler();
};

#endif