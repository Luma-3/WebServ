/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:46:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/26 10:29:33 by jbrousse         ###   ########.fr       */
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
#include "Logger.hpp"
#include "VirtualServer.hpp"

class Handler
{
  private:
	const char						**_envp;
	const int						  _epfd;
	int								  _nbServ;
	std::map< std::string, int >	  _hostptofd;
	std::map< int, ServerHost * >	  _servers;
	std::map< int, client::Client * > _clients;

	void handleNewConnection(const ServerHost *server);
	void handleClientRequest(int event_fd, const std::string &request = "");
	void handleClientResponse(int event_fd);
	void handleClientDisconnection(int event_fd);
	void handleNewClient(const ServerHost *server, int client_socket,
						 sockaddr_storage  *client_addr,
						 const std::string &request);

	void addEvent(int fd, uint32_t events) const;
	void removeEvent(int fd) const;
	void modifyEvent(int fd, uint32_t events) const;

  public:
	Handler(const std::vector< VirtualServer * > &servers, const char **envp);

	void runEventLoop();

	~Handler();
};

#endif