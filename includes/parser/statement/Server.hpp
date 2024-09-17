/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:04:03 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 13:09:43 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

namespace statement {

#include <vector>

#include "lexer/Token.hpp"
#include "parser/statement/ErrorPage.hpp"
#include "parser/statement/Location.hpp"

class Server : public Token
{
  private:
	std::string				   _port;
	std::string				   _server_name;
	std::string				   _root;
	std::string				   _index;
	bool					   _autoindex;
	std::string				   _return;
	std::vector< std::string > _deny_methods;

	std::vector< statement::ErrorPage >		   _error_pages;
	std::vector< const statement::Location * > _locations;

  public:
	Server();
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();
};

Server::Server() {}

Server::Server(const Server &src) {}

Server &Server::operator=(const Server &src)
{
	if (this != &src) {}
	return *this;
}

Server::~Server() {}

} // namespace statement

#endif // SERVERCONFIG_HPP