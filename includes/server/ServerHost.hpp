/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHost.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 16:37:15 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/13 12:50:04 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERHOST_HPP
#define SERVERHOST_HPP

#include <map>
#include <string>

#include "VirtualServer.hpp"

#define MAXREQUEST	 10
#define MAX_REQ_SIZE 1024

class ServerHost
{
  private:
	VirtualServer										*_default_vhost;
	std::map< const std::string, const VirtualServer * > _vhost;
	int													 _nbVhost;
	int													 _socket;

	struct addrinfo *setupSocket(const std::string &host,
								 const std::string &port,
								 struct addrinfo   *hints);
	void			 bindAndListenSocket(struct addrinfo *info);

  public:
	ServerHost();
	ServerHost(const std::string &host, const std::string &port);
	ServerHost(const ServerHost &src);

	ServerHost &operator=(const ServerHost &rhs);
	bool		operator==(const ServerHost &rhs) const;

	~ServerHost();

	int					 getSocket() const { return _socket; };
	int					 getNbVhost() const { return _nbVhost; };
	const VirtualServer *getVhost(const std::string &host_name) const;
	const VirtualServer *getDefaultVhost() const;

	void AddServer(std::string host_name, VirtualServer *server);

	std::string recvRequest(int client_socket) const;
	void sendResponse(int client_socket, const std::string &response) const;
	int	 acceptClient() const;
};

#endif // SERVERHOST_HPP