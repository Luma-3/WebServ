/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:46:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/09 09:57:16 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <vector>

#include "Server.hpp"

class WebServ
{
  private:
	std::vector<Server> _servers;

  public:
	WebServ();
	~WebServ();
};

#endif // WEBSERV_HPP