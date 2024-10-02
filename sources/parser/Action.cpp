/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 14:05:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/02 23:26:48 by jbrousse         ###   ########.fr       */
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
#include "parser/statement/ReturnParam.hpp"
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
	else if (_type == ERROR) {
		return (2);
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

void parser::R1(std::stack< Token * > &stack)
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

void parser::R2(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;

	for (size_t i = 0; i < 4; ++i) {
		tokens.push_back(stack.top());
		stack.pop();
	}

	statement::Param *param =
		new statement::Param(tokens[1]->getValue(), tokens[3]->getTerminal());

	for (size_t i = 0; i < tokens.size(); ++i) {
		delete tokens[i];
	}
	stack.push(param);
}

void parser::R3(std::stack< Token * > &stack)
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

void parser::R4(std::stack< Token * > &stack)
{
	std::stack< Token * > tokens;

	TakeTo(tokens, stack, Token::IsKey);

	std::string				   value;
	std::vector< std::string > errorCode;

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_ErrorCode) {
			errorCode.push_back(tokens.top()->getValue());
		}
		else if (tokens.top()->getTerminal() == T_File) {
			value = tokens.top()->getValue();
		}
		delete tokens.top();
		tokens.pop();
	}

	statement::ErrorPage *error = new statement::ErrorPage(errorCode, value);

	stack.push(error);
}

void parser::R5(std::stack< Token * > &stack)
{
	std::stack< Token * > tokens;

	TakeTo(tokens, stack, Token::IsKey);

	std::string value;
	std::string code;

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_Value ||
			tokens.top()->getTerminal() == T_Path ||
			tokens.top()->getTerminal() == T_File) {
			value = tokens.top()->getValue();
		}
		else if (tokens.top()->getTerminal() == T_ErrorCode) {
			code = tokens.top()->getValue();
		}
		delete tokens.top();
		tokens.pop();
	}

	statement::ReturnParam *returnParam =
		new statement::ReturnParam(code, value);

	stack.push(returnParam);
}

void parser::R6(std::stack< Token * > &stack)
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