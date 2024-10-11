/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 14:05:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/11 10:08:55 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Action.hpp"

#include <cstddef>
#include <cstdlib>
#include <stack>
#include <string>
#include <vector>

#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/statement/DenyMethod.hpp"
#include "parser/statement/ErrorPage.hpp"
#include "parser/statement/Location.hpp"
#include "parser/statement/Param.hpp"
#include "parser/statement/ParamDouble.hpp"
#include "parser/statement/Server.hpp"

using parser::Action;

// TODO : clean useless includes

Action::Action() : _type(ERROR), _next_state(-1), _rule_func(NULL) {}

Action::Action(const Action &src) :
	_type(src._type),
	_next_state(src._next_state),
	_rule_func(src._rule_func)
{
}

Action::Action(ActionType type, int next_states,
			   void (*rule_func)(std::stack< Token * > &)) :
	_type(type),
	_next_state(next_states),
	_rule_func(rule_func)
{
}

Action::Action(ActionType type, int next_states) :
	_type(type),
	_next_state(next_states),
	_rule_func(NULL)
{
}

Action &Action::operator=(const Action &src)
{
	if (this != &src) {
		_type = src._type;
		_next_state = src._next_state;
		_rule_func = src._rule_func;
	}
	return *this;
}

void Action::Shift(Token *token, std::stack< Token * > &stack,
				   Parser &parser) const
{
	Token *currentToken = token;

	stack.push(currentToken);
	parser.setState(_next_state);
}

void Action::Reduce(Token *token, std::stack< Token * > &stack,
					Parser &parser) const
{
	Shift(token, stack, parser);
	_rule_func(stack);
}

int Action::Execute(Token *token, std::stack< Token * > &stack,
					Parser &parser) const
{
	if (_type == SHIFT) {
		Shift(token, stack, parser);
	}
	else if (_type == REDUCE) {
		Reduce(token, stack, parser);
	}
	else {
		return (ERROR);
	}
	return (0);
}

Action::~Action() {}

void TakeTo(std::stack< Token * > &dst, std::stack< Token * > &stack,
			bool (*breaker)(const Token &))
{
	while (!stack.empty()) {
		if (breaker(*stack.top())) {
			break;
		}
		dst.push(stack.top());
		stack.pop();
	}
	dst.push(stack.top());
	stack.pop();
}

bool R1Breaker(const Token &token)
{
	return (token.getTerminal() == T_Server);
}

bool R6Breaker(const Token &token)
{
	return (token.getTerminal() == T_Location);
}

void parser::R1_Server(std::stack< Token * > &stack)
{
	std::stack< Token * >  tokens;
	std::vector< Token * > params;

	TakeTo(tokens, stack, R1Breaker);

	while (!tokens.empty()) {

		if (tokens.top()->getType() != S_Terminal) {
			params.push_back(tokens.top());
		}
		else {
			delete tokens.top();
		}
		tokens.pop();
	}

	statement::Server *server = new statement::Server(params);
	stack.push(server);
}

void parser::R2_Param(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;

	for (size_t i = 0; i < 3; ++i) {
		tokens.push_back(stack.top());
		stack.pop();
	}

	Terminal_Type term_type = tokens[2]->getTerminal();
	std::string	  value = tokens[1]->getValue();

	if (term_type == T_Host) {
		if (!IsHost(value)) {
			throw Token::InvalidTokenException();
		}
	}
	else if (term_type == T_Port) {
		if (!IsPort(value)) {
			throw Token::InvalidTokenException();
		}
	}
	else if (term_type == T_BodySize) {
		if (!IsBodySize(value)) {
			throw Token::InvalidTokenException();
		}
	}

	statement::Param *param = new statement::Param(value, term_type);

	for (size_t i = 0; i < tokens.size(); ++i) {
		delete tokens[i];
	}
	stack.push(param);
}

void parser::R3_DoubleParam(std::stack< Token * > &stack)
{
	std::stack< Token * > tokens;

	TakeTo(tokens, stack, Token::IsKey);

	std::string value1;
	std::string value2;

	Terminal_Type term_type = tokens.top()->getTerminal();
	Token_Type	  type;

	while (!tokens.empty()) {
		if (term_type == T_Return) {
			type = S_Return;
			if (tokens.top()->getTerminal() == T_Digits) {
				if (!IsErrorCode(tokens.top()->getValue())) {
					throw Token::InvalidTokenException();
				}
				value1 = tokens.top()->getValue();
			}
			else if (tokens.top()->getTerminal() == T_Identifier ||
					 tokens.top()->getTerminal() == T_Path) {
				value2 = tokens.top()->getValue();
			}
		}
		else {
			type = S_Log;
			if (tokens.top()->getTerminal() == T_LogLevel) {
				value1 = tokens.top()->getValue();
			}
			else if (tokens.top()->getTerminal() == T_Identifier) {
				value2 = tokens.top()->getValue();
			}
		}
		delete tokens.top();
		tokens.pop();
	}

	statement::ParamDouble *returnParam =
		new statement::ParamDouble(value1, value2, type);

	stack.push(returnParam);
}

void parser::R4_ErrorPage(std::stack< Token * > &stack)
{
	std::stack< Token * > tokens;

	TakeTo(tokens, stack, Token::IsKey);

	std::string				   value;
	std::vector< std::string > errorCode;

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_Digits) {
			if (!IsErrorCode(tokens.top()->getValue())) {
				throw Token::InvalidTokenException();
			}
			errorCode.push_back(tokens.top()->getValue());
		}
		else if (tokens.top()->getTerminal() == T_Identifier) {
			value = tokens.top()->getValue();
		}
		delete tokens.top();
		tokens.pop();
	}

	statement::ErrorPage *error = new statement::ErrorPage(errorCode, value);

	stack.push(error);
}

void parser::R5_DenyMethod(std::stack< Token * > &stack)
{
	std::stack< Token * >	   tokens;
	std::vector< std::string > method;

	TakeTo(tokens, stack, Token::IsKey);

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_Method) {
			method.push_back(tokens.top()->getValue());
		}
		delete tokens.top();
		tokens.pop();
	}

	statement::DenyMethod *deniedMethod = new statement::DenyMethod(method);
	stack.push(deniedMethod);
}

void parser::R6_Location(std::stack< Token * > &stack)
{
	std::stack< Token * > tokens;

	TakeTo(tokens, stack, R6Breaker);

	std::string			   value;
	std::vector< Token * > params;

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_Path) {
			value = tokens.top()->getValue();
			delete tokens.top();
		}
		else if (tokens.top()->getType() != S_Terminal) {
			params.push_back(tokens.top());
		}
		else {
			delete tokens.top();
		}
		tokens.pop();
	}

	statement::Location *location = new statement::Location(params, value);

	stack.push(location);
}