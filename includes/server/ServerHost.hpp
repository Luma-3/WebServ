/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHost.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 16:37:15 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/27 11:07:36 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERHOST_HPP
#define SERVERHOST_HPP

#include <map>
#include <string>
#include <sys/socket.h>

#include "server/ServerException.hpp"
#include "VirtualServer.hpp"

#define MAXREQUEST	 10
#define MAX_REQ_SIZE 1024
#define CHUNK_SIZE	 1024

class ServerHost
{
  private:
	VirtualServer										*_default_vhost;
	std::map< const std::string, const VirtualServer * > _vhost;
	int													 _nbVhost;
	int													 _socket;

	void setupSocket(const struct addrinfo *info);
	void bindAndListenSocket(const struct addrinfo *info) const;

  public:
	ServerHost();
	ServerHost(struct addrinfo *info);
	ServerHost(const ServerHost &src);

	ServerHost &operator=(const ServerHost &rhs);
	bool		operator==(const ServerHost &rhs) const;

	~ServerHost();

	int					 getSocket() const { return _socket; };
	int					 getNbVhost() const { return _nbVhost; };
	const VirtualServer *getVhost(const std::string &server_name) const;
	const VirtualServer *getDefaultVhost() const;

	void AddServer(const std::string &server_name, VirtualServer *server);

	static void sendResponse(int client_socket, const std::string &response);
	static std::string recvRequest(int client_socket);
	int				   acceptClient(sockaddr_storage *client_addr) const;
};

#endif // SERVERHOST_HPP
