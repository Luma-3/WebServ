/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:55:45 by anthony           #+#    #+#             */
/*   Updated: 2024/09/10 17:30:13 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>

using std::string;

class Server {
protected:
    std::map<string, std::map<string, string> > _locations;

public:
    Server() {
		string root_ = "root";
		string index_ = "/index";
		string root = "/var/www/html";
		string index = "index.html";
		string error = "404";
		
        // Initialisation de la _locationsuration
        _locations[index_][root_] = root;
        _locations[error][root_] = root;
    }
};