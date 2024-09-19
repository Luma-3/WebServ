/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:28:51 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/19 10:53:18 by jbrousse         ###   ########.fr       */
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

static Action findAction(int state, Terminal_Type terminal)
{
	for (size_t i = 0; i < NB_ACTIONS; ++i) {
		if (g_action[i].state == state && g_action[i].terminal == terminal) {
			return g_action[i].action;
		}
	}
	return g_action[NB_ACTIONS - 1].action;
}

void Parser::Parse()
{
	for (size_t i = 0; i < _tokens.size(); ++i) {
		Token *token = _tokens[i];
		Action action = findAction(_status, token->getTerminal());
		action.Execute(token, _parse_stack, *this);
	}
}

Parser::~Parser()
{
	// TODO : Peut-etre
}