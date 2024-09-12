/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:55:45 by anthony           #+#    #+#             */
/*   Updated: 2024/09/12 11:21:57 by Monsieur_Ca      ###   ########.fr       */
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
		string root = "www";
		string index = "index.html";
		string error404 = "404.html";
		// string error500 = "500.html";

		_locations["/"]["root"] = root;
		_locations["/"]["index"] = root + "/" + index;
		_locations["/404"]["root"] = root;
		_locations["/404"]["file"] = root + "/" + error404;
		// _locations["/500"]["root"] = root;
		// _locations["/500"]["file"] = root + "/" + error500;
	}
};