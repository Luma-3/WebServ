/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:22:15 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/04 13:01:39 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "client/Builder.hpp"
#include "client/Parser.hpp"
#include "server/Server.hpp"

namespace client {

class Client
{
  private:
	const Server *_server;
	const Server *_default_server;
	const int	  _client_socket;
	std::string	  _request;
	std::string	  _response;

  public:
	Client();
	Client(const Server *server, const Server *default_s, int client_socket);
	Client(const Client &src);
	Client &operator=(const Client &src);
	~Client();

	int				   getSocket() const;
	const Server	  *getServer() const;
	const std::string &getRequest() const;

	void receiveRequest();
	void sendResponse();

	void handleRequest();
};

} // namespace client

#endif // CLIENT_HPP