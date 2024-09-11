/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:28:25 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 11:48:54 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

#include "Location.hpp"

class Config
{
  private:
	std::string			   _port;
	std::string			   _server_name;
	std::vector<Locations> _locations;

  public:
	Config();
	~Config();
	Config(const Config &);
	Config &operator=(const Config &);
};

#endif // CONFIG_HPP