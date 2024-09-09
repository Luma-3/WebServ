/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:56:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/09 12:33:27 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netdb.h>
#include <string>

class Server
{
  private:
	const std::string _port;
	const int		  _server_socket;
	struct addrinfo	 *_info;

  public:
	Server(/* args */);
	~Server();

	void ListenConnexion();
};

#endif // SERVER_HPP