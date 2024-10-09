/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:46:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/06 22:54:51 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
#define HANDLER_HPP

#define MAX_EVENTS 10

#include <csignal>
#include <exception>
#include <sys/epoll.h>
#include <vector>

#include "Server.hpp"
// # include "Config.hpp"

#include "lexer/Token.hpp"

class Handler
{
  private:
	std::vector< Server * > _servers;
	int						_epfd;

  public:
	Handler();
	Handler(const std::vector< statement::Server * > &servers);
	Handler(const Handler &src);
	Handler &operator=(const Handler &rhs);

	void loadServTest();
	int	 launchServers();
	int	 handleEvents();

	// Server *operator[](const int index);

	~Handler();
};

#endif