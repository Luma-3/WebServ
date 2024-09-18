/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:28:51 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 19:08:55 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Parser.hpp"

#include "parser/Action.hpp"

Parser::Parser() : _status(0) {};

Parser::Parser(const Parser &src) : _status(src._status) {}

Parser::Parser(const std::vector< Token * > &tokens) :
	_tokens(tokens),
	_status(0)
{
}

Parser &Parser::operator=(const Parser &src)
{
	if (this != &src) {
		_parse_stack = src._parse_stack;
	}
	return *this;
}

void Parser::setState(int state)
{
	_status = state;
}

const std::stack< Token * > &Parser::getParseStack() const
{
	return _parse_stack;
}

void Parser::Parse()
{
	for (size_t i = 0; i < _tokens.size(); ++i) {
		Token *token = _tokens[i];
		for (size_t j = 0; j < NB_ACTIONS; ++j) {
			if (g_action[j].state == _status &&
				g_action[j].terminal == token->getTerminal()) {
				g_action[j].action.Execute(token, _parse_stack, *this);
				break;
			}
			std::cout << "j: " << j << " | terminal: " << token->getTerminal()
					  << " | state: " << _status << std::endl;
		}
	}
}

Parser::~Parser()
{
	// TODO : Peut-etre
}