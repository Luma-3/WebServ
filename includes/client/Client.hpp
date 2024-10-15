/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:22:15 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/15 12:59:57 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "client/Builder.hpp"
#include "client/Parser.hpp"
#include "server/ServerHost.hpp"
#include "server/VirtualServer.hpp"

#define MAX_REQ_SIZE 1024
#define PACKET_SIZE	 1024

namespace client {

class Client
{
  private:
	const VirtualServer *_server;
	const VirtualServer *_default_server;
	const ServerHost	*_host;
	const int			 _client_socket;

	std::string _request;
	std::string _response;
	std::string _header;
	std::string _body;
	bool		_autoindex;
	std::string _autoindex_path;

  public:
	Client();
	Client(const VirtualServer *server, const VirtualServer *default_s,
		   int client_socket);
	Client(const Client &src);
	Client &operator=(const Client &src);
	~Client();

	bool operator==(const Client &rhs) const;

	const ServerHost  *getHost() const;
	int				   getSocket() const;
	const std::string &getRequest() const;
	const std::string &getResponse() const { return _response; };
	const std::string &getBody() const;

	void setRequest(const std::string &request) { _request = request; };

	void handleRequest();
};

} // namespace client

#endif // CLIENT_HPP