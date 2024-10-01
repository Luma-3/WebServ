/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:22:15 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/01 10:03:49 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "client/Parser.hpp"
#include "server/Server.hpp"

namespace client {

class Client
{
  private:
	const Server *_server;
	const int	  _client_socket;
	std::string	  _request;

  public:
	Client();
	Client(const Server *server, int client_socket);
	Client(const Client &src);
	Client &operator=(const Client &src);
	~Client();

	int				   getSocket() const;
	const Server	  *getServer() const;
	const std::string &getRequest() const;

	void receiveRequest();
	void sendResponse(const std::string &response);

	void ParseRequest(client::Parser &parser);
};

} // namespace client

#endif // CLIENT_HPP