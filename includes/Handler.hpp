/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:46:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/12 23:43:37 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
# define HANDLER_HPP

# define MAX_EVENTS 10

# include <vector>
# include <sys/epoll.h>
# include <exception>

# include "Server.hpp"

// # include "Config.hpp"

typedef typename std::vector<Server>::iterator handlerIt;


class Handler
{
  private:
	std::vector<Server*>	_servers;
	int						_nbServ;
	
  public:
	Handler();

	void					loadServTest( );
	std::vector<Server*>	getAllServ( void) const;
	int						getNbServ( void) const;

	Server					*operator[]( const int index);

	// handlerIt	begin( void)
	// {
	// 	return (this->_servers.begin());
	// }
	// handlerIt	end( void)
	// {
	// 	return (this->_servers.end());
	// }
	

	~Handler();
};

#endif 