/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:28:51 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/11 19:47:33 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Parser.hpp"

Parser::Parser() : _status(0) {};

Parser::Parser(const Parser &src)
{
	_parse_stack = src._parse_stack;
}

Parser::Parser(const char *file_path) :
	_config_file(file_path, std::ios::in),
	_status(0)
{
	if (!_config_file.is_open()) {
		std::cerr << "Failed to open config file: " << file_path << std::endl;
	}
}

Parser &Parser::operator=(const Parser &src)
{
	if (this != &src) {
		_parse_stack = src._parse_stack;
	}
	return *this;
}

// bool Parser::isErrorCode(std::string &val)
// {
// 	if (val.size() != 3) {
// 		return false;
// 	}
// 	for (size_t i = 0; i < val.size(); i++) {
// 		if (val[i] < '0' || val[i] > '9') {
// 			return false;
// 		}
// 	}
// 	return true;
// }

// bool Parser::isPort(std::string &val)
// {
// 	for (size_t i = 0; i < val.size(); i++) {
// 		if (!isdigit(val[i])) {
// 			return false;
// 		}
// 	}
// 	return true;
// }

// bool Parser::isBool(std::string &val)
// {
// 	return (val == "on" || val == "off");
// }

Parser::~Parser()
{
	if (_config_file.is_open()) {
		_config_file.close();
	}
}