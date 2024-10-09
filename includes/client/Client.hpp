/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:22:15 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/09 15:00:32 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "client/Builder.hpp"
#include "client/Parser.hpp"
#include "server/Server.hpp"

#define MAX_REQ_SIZE 1024
#define PACKET_SIZE	 1024

namespace client {

class Client
{
  private:
	const Server *_server;
	const Server *_default_server;
	const int	  _client_socket;
	std::string	  _request;
	std::string _response;
	std::string	  _header;
	std::string	  _body;

  public:
	Client();
	Client(const Server *server, const Server *default_s, int client_socket);
	Client(const Client &src);
	Client &operator=(const Client &src);
	~Client();

	int				   getSocket() const;
	const Server	  *getServer() const;
	const std::string &getRequest() const;
	const std::string &getBody() const;

	void receiveRequest();
	void sendResponse();

	void handleRequest();
};

} // namespace client

#endif // CLIENT_HPP