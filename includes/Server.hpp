/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:56:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/12 23:32:53 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netdb.h>
#include <string>
#include <string.h>
# include <cerrno>

class Server
{
  protected:
  	const std::string	_name;
	const std::string	_hostname;
	const std::string 	_port;
	const int		  	_server_socket;
	int					_new_socket;
	struct addrinfo	 	*_info;
	

  public:
	Server( std::string servername, std::string hostname, std::string port);

	int			getSocket( void) const;
	std::string	getName( void) const;

	void	createSocket();
	void 	HandleConnexion();

	~Server();
};

#endif // SERVER_HPP