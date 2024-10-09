/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:56:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/09 10:52:01 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

#include "Logger.hpp"
#include "parser/statement/ErrorPage.hpp"
#include "parser/statement/Location.hpp"
#include "parser/statement/Server.hpp"

#define SUCCESS 0
#define FAILURE 1

#define MAXREQUEST 10
class Server
{
  private:
	const int _socket;
	Logger	 *_logger;

	std::string		  _name;
	const std::string _hostname;
	const std::string _port;

	const Server							   *_default;
	const std::string							_root;
	const std::string							_index;
	const bool									_autoindex;
	const statement::ParamDouble				_returns;
	const std::vector< std::string >			_deny_methods;
	std::vector< const statement::ErrorPage * > _error_pages;
	std::vector< const statement::Location * >	_locations;

	// Constructor anex
	void setSocketOptions();
	void bindAndListenSocket();

  public:
	Server();
	Server(const statement::Server *server, const Server *default_server);
	Server(const Server &src);
	Server &operator=(const Server &src);

	std::string getName() const;
	std::string getHost() const;
	std::string getPort() const;

	int getSocket() const;

	const std::string &getRoot() const;
	const std::string &getIndex() const;
	bool			   getAutoindex() const;

	const std::vector< std::string >				  &getDenyMethods() const;
	const std::vector< const statement::ErrorPage * > &getErrorPages() const;
	const std::vector< const statement::Location * >  &getLocations() const;
	const statement::ParamDouble					  &getReturns() const;

	const Server *getDefault() const;

	int acceptRequest() const;

	virtual ~Server();
};

#include "template/vector_deep_copy.tpp"

#endif