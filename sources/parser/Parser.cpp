/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:28:51 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 16:05:56 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(std::string file_path) :
	_config_file(file_path.c_str(), std::ios::in)
{
	if (_config_file.is_open() == false) {
		std::cerr << "Failed to open config file: " << file_path << std::endl;
	}

	Tokenize(_config_file);
}

bool Parser::isErrorCode(std::string &val)
{
	if (val.size() != 3) return false;
	for (size_t i = 0; i < val.size(); i++) {
		if (val[i] < '0' || val[i] > '9') return false;
	}
	return true;
}

bool Parser::isPort(std::string &val)
{
	for (size_t i = 0; i < val.size(); i++) {
		if (!isdigit(val[i])) return false;
	}
	return true;
}

bool Parser::isBool(std::string &val)
{
	if (val == "on" || val == "off") return true;
	return false;
}

Parser::~Parser()
{
	if (_config_file.is_open() == true) _config_file.close();
}