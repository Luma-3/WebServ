/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:05:05 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/09 15:06:43 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <fstream>
#include <map>
#include <vector>

// #include "Server.hpp"

class WebServ
{
  private:
	// std::vector<Server>				   _servers;
	std::ifstream					   _config_file;
	std::map<std::string, std::string> _parse;

  public:
	WebServ(std::string file_path);

	void Parse();

	~WebServ();
};

#endif // WEBSERV_HPP