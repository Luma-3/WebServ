/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:04:03 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/07 18:11:11 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSTATEMENT_HPP
#define SERVERSTATEMENT_HPP

#include <ostream>
#include <vector>

#include "lexer/Token.hpp"
#include "parser/statement/ErrorPage.hpp"
#include "parser/statement/Location.hpp"
#include "parser/statement/ParamDouble.hpp"

namespace statement {

class Server : public Token
{
  private:
	std::string				   _name;
	std::string				   _port;
	std::string				   _host;
	std::string				   _root;
	std::string				   _index;
	std::string				   _body_size;
	bool					   _autoindex;
	statement::ParamDouble	   _return;
	statement::ParamDouble	   _log;
	std::vector< std::string > _deny_methods;

	std::vector< const statement::ErrorPage * > _error_pages;
	std::vector< const statement::Location * >	_locations;

  public:
	Server();
	Server(const Server &src);
	Server(const std::vector< Token * > &tokens);
	Server &operator=(const Server &src);
	~Server();

	const std::string &getName() const
	{
		return _name;
	}
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

	const std::string &getBodySize() const
	{
		return _body_size;
	}
	const statement::ParamDouble &getLog() const
	{
		return _log;
	}

	bool getAutoindex() const
	{
		return _autoindex;
	}
	const statement::ParamDouble &getReturn() const
	{
		return _return;
	}
	const std::vector< std::string > &getDenyMethods() const
	{
		return _deny_methods;
	}
	const std::vector< const statement::ErrorPage * > &getErrorPages() const
	{
		return _error_pages;
	}
	const std::vector< const statement::Location * > &getLocations() const
	{
		return _locations;
	}

	bool operator==(const Server &rhs) const;

  private:
	void IdentifyParam(Token *token);
};

} // namespace statement

std::ostream &operator<<(std::ostream &o, const statement::Server &server);

#include "template/Cast.tpp"

#endif // SERVERSTATEMENT_HPP