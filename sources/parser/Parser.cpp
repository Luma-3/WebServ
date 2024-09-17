/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:28:51 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 13:17:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Parser.hpp"

Parser::Parser() : _status(0) {};

Parser::Parser(const Parser &src) : _status(src._status) {}

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

const std::vector< Token * > &Parser::getTokens() const
{
	return _tokens;
}

void Parser::setState(int state)
{
	_status = state;
}

Parser::~Parser()
{
	if (_config_file.is_open()) {
		_config_file.close();
	}
}