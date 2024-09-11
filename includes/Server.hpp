/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:55:45 by anthony           #+#    #+#             */
/*   Updated: 2024/09/11 14:14:28 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>

using std::string;

class Server
{
  protected:
	std::map< string, std::map< string, string > > _locations;

  public:
	Server()
	{
		string root_ = "root";
		string index_ = "/index";
		string root = "/var/www/html";
		string index = "index.html";
		string error = "404";

		// Initialisation de la _locationsuration
		_locations[index_][root_] = root;
		_locations[index_][index_] = root + "/" + index;
		_locations[error][root_] = root;
	}
};