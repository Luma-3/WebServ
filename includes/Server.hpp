/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:55:45 by anthony           #+#    #+#             */
/*   Updated: 2024/09/20 17:15:58 by Monsieur_Ca      ###   ########.fr       */
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
		string html_root = root + "/html";
		string css_root = root + "/css";
		string images_root = root + "/images";
		string index = "index.html";
		string error404 = "404.html";
		// string error500 = "500.html";

		// Configuration pour les fichiers HTML
		_locations["/"]["root"] = html_root;
		_locations["/"]["index"] = html_root + "/" + index;

		// Configuration pour les fichiers CSS
		_locations["/css"]["root"] = css_root;

		// Configuration pour les images
		_locations["/images"]["root"] = images_root;

		_locations["/icons"]["root"] = images_root;
	}
};
