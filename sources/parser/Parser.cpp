/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:28:51 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/24 15:07:59 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Parser.hpp"

#include <cstddef>
#include <stack>
#include <vector>

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/Action.hpp"

using parser::Action;
using parser::Parser;

Parser::Parser() : _lexer(NULL), _status(0) {};

Parser::Parser(const Parser &src) : _lexer(src._lexer), _status(src._status) {}

Parser::Parser(Lexer *lexer) : _lexer(lexer), _status(0) {}

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

const Lexer &Parser::getLexer() const
{
	return *_lexer;
}

const int &Parser::getState() const
{
	return _status;
}

/**
 * This namespace is used to hide the findAction and findExpected functions
 * is more appropriate to use a namespace than a static keyword in modern C++
 * (cpp98 is ok with this)
 */
namespace {

Action findAction(int state, Terminal_Type terminal)
{
	for (size_t i = 0; i < NB_ACTIONS; ++i) {
		if (parser::g_action[i].state == state &&
			parser::g_action[i].terminal == terminal) {
			return parser::g_action[i].action;
		}
	}
	return parser::g_action[NB_ACTIONS - 1].action;
}

struct parser::ActionEntry findExpected(int state)
{
	for (size_t i = 0; i < NB_ACTIONS; ++i) {
		if (parser::g_action[i].state == state) {
			return (parser::g_action[i]);
		}
	}
	return (parser::g_action[NB_ACTIONS - 1]);
}

} // namespace

void Parser::Parse()
{
	for (size_t i = 0; i < _lexer->getTokens().size(); ++i) {
		Token		*token = _lexer->getTokens()[i];
		const Action action = findAction(_status, token->getTerminal());

		if (action.Execute(token, _parse_stack, *this) == ERROR) {
			throw InvalidTokenException(
				token->getCol(), token->getLine(), token->getValue(),
				Token::TerminalTypeToString(findExpected(_status).terminal));
		}
	}
}

Parser::~Parser()
{
	while (!_parse_stack.empty()) {
		if (_parse_stack.top()->getType() != S_Terminal) {
			delete _parse_stack.top();
		}
		_parse_stack.pop();
	}
}