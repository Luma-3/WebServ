/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:17:47 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 20:07:19 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <WebServ.hpp>

WebServ::WebServ(std::string file_path) :
	_config_file(file_path.c_str(), std::ios::in)
{
	if (_config_file.is_open() == false) {
		std::cerr << "Failed to open config file" << file_path << std::endl;
	}
}

void WebServ::Parse()
{
	char *buff = new char[10];

	while (_config_file.read(buff, 10)) {
		std::cout << buff << std::endl;
	}
}

WebServ::~WebServ()
{
	if (_config_file.is_open() == true) _config_file.close();
}