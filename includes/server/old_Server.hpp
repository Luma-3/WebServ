/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:56:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/11 22:38:07 by jbrousse         ###   ########.fr       */
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
class old_Server
{
  private:
	const std::string _ip;
	const std::string _port;
	const std::string _hostname;

	int		_socket;
	Logger *_logger;

	const std::string							_root;
	const std::string							_index;
	const bool									_autoindex;
	const statement::ParamDouble				_returns;
	const std::vector< std::string >			_deny_methods;
	std::vector< const statement::ErrorPage * > _error_pages;
	std::vector< const statement::Location * >	_locations;

	// Constructor anex
	void setSocketOptions();
	void bindAndListenSocket(struct addrinfo *info);

  public:
	old_Server();
	old_Server(const statement::Server *server,
			   const old_Server		   *default_server);
	old_Server(const old_Server &src);
	old_Server &operator=(const old_Server &src);

	std::string										   getName() const;
	std::string										   getHost() const;
	std::string										   getPort() const;
	int												   getSocket() const;
	bool											   getAutoindex() const;
	const std::string								  &getRoot() const;
	const std::string								  &getIndex() const;
	const std::vector< std::string >				  &getDenyMethods() const;
	const std::vector< const statement::ErrorPage * > &getErrorPages() const;
	const std::vector< const statement::Location * >  &getLocations() const;
	const statement::ParamDouble					  &getReturns() const;
	const old_Server								  *getDefault() const;

	int acceptRequest() const;

	virtual ~old_Server();
};

#include "template/vector_deep_copy.tpp"

#endif