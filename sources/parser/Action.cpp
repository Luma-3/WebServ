/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 14:05:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/19 10:57:31 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Action.hpp"

#include <iostream>
#include <stack>
#include <string>

#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/statement/DenyMethod.hpp"
#include "parser/statement/ErrorPage.hpp"
#include "parser/statement/Location.hpp"
#include "parser/statement/Param.hpp"
#include "parser/statement/ReturnParam.hpp"
#include "parser/statement/Server.hpp"

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
	stack.push(token);
	parser.setState(_next_state);
}

void Action::Reduce(Token *token, std::stack< Token * > &stack,
					Parser &parser) const
{
	Shift(token, stack, parser);
	_rule_func(stack);
}

void Action::Execute(Token *token, std::stack< Token * > &stack,
					 Parser &parser) const
{
	if (_type == SHIFT) {
		Shift(token, stack, parser);
	} else if (_type == REDUCE) {
		Reduce(token, stack, parser);
	} else if (_type == ERROR) {
		std::cout << "Error" << std::endl;
		throw std::exception();
	}
}

Action::~Action() {}

void R1(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;

	do {
		tokens.push_back(stack.top());
		stack.pop();
	} while (!Token::IsKey(*tokens.back()) ||
			 tokens.back()->getType() != S_Terminal);

	std::vector< Token * > params;

	size_t i = 1;
	while (i < tokens.size() && tokens[i]->getTerminal() != T_OBracket) {
		params.push_back(tokens[i]);
		++i;
	}

	statement::Server *server = new statement::Server(params);

	stack.push(server);
}

void R2(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;

	for (size_t i = 0; i < 4; ++i) {
		tokens.push_back(stack.top());
		stack.pop();
	}

	statement::Param *param =
		new statement::Param(tokens[1]->getValue(), tokens[3]->getTerminal());

	stack.push(param);
}

void R3(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;

	do {
		tokens.push_back(stack.top());
		stack.pop();
	} while (!Token::IsKey(*tokens.back()));

	std::vector< std::string > method;

	for (size_t i = 1; i < tokens.size(); i += 2) {
		if (Token::IsKey(*tokens[i])) {
			break;
		}
		method.push_back(tokens[i]->getValue());
	}

	statement::DenyMethod *deniedMethod = new statement::DenyMethod(method);

	stack.push(deniedMethod);
}

void R4(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;

	do {
		tokens.push_back(stack.top());
		stack.pop();
	} while (!Token::IsKey(*tokens.back()));

	std::string				   value = tokens[1]->getValue();
	std::vector< std::string > errorCode;

	for (size_t i = 3; i < tokens.size(); i += 2) {
		if (Token::IsKey(*tokens[i])) {
			break;
		}
		errorCode.push_back(tokens[i]->getValue());
	}

	statement::ErrorPage *error = new statement::ErrorPage(errorCode, value);

	stack.push(error);
}

void R5(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;

	do {
		tokens.push_back(stack.top());
		stack.pop();
	} while (!Token::IsKey(*tokens.back()));

	std::string value = tokens[1]->getValue();

	std::string code = tokens[3]->getValue();

	statement::ReturnParam *returnParam =
		new statement::ReturnParam(code, value);

	stack.push(returnParam);
}

void R6(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;

	do {
		tokens.push_back(stack.top());
		stack.pop();
	} while (!Token::IsKey(*tokens.back()) ||
			 tokens.back()->getType() != S_Terminal);

	std::string value = tokens[tokens.size() - 1]->getValue();

	std::vector< Token * > params;

	size_t i = 3;
	while (i < tokens.size() && tokens[i]->getTerminal() != T_OBracket) {
		params.push_back(tokens[i]);
		++i;
	}

	statement::Location *location = new statement::Location(params, value);

	stack.push(location);
}