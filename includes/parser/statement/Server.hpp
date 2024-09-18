/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:04:03 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 16:27:24 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <ostream>
#include <vector>

#include "lexer/Token.hpp"
#include "parser/statement/ErrorPage.hpp"
#include "parser/statement/Location.hpp"

namespace statement {

class Server : public Token
{
  private:
	std::string				   _port;
	std::string				   _host;
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
	Server(const std::vector< Token * > &tokens);
	Server &operator=(const Server &src);
	~Server();

	const std::string &getPort() const
	{
		return _port;
	}
	const std::string &getHost() const
	{
		return _host;
	}
	const std::string &getRoot() const
	{
		return _root;
	}
	const std::string &getIndex() const
	{
		return _index;
	}
	bool getAutoindex() const
	{
		return _autoindex;
	}
	const std::string &getReturn() const
	{
		return _return;
	}
	const std::vector< std::string > &getDenyMethods() const
	{
		return _deny_methods;
	}
	const std::vector< statement::ErrorPage > &getErrorPages() const
	{
		return _error_pages;
	}
	const std::vector< const statement::Location * > &getLocations() const
	{
		return _locations;
	}

  private:
	void IdentifyParam(Token *token);
};

} // namespace statement

std::ostream &operator<<(std::ostream &o, const statement::Server &server);

#endif // SERVERCONFIG_HPP