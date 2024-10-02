/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 14:05:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/02 13:18:21 by jbrousse         ###   ########.fr       */
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
	} else if (_type == REDUCE) {
		Reduce(token, stack, parser);
	} else if (_type == ERROR) {
		return (2);
	}
	return (0);
}

Action::~Action() {}

void deleteTmp(std::vector< Token * > &tokens)
{
	for (size_t i = 0; i < tokens.size(); ++i) {
		std::cout << "Delete : " << *tokens[i] << std::endl;
		delete tokens[i];
	}
}

void parser::R1(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;
	Token				  *currentToken = stack.top();
	stack.pop();

	while (!Token::IsKey(*currentToken) ||
		   currentToken->getType() != S_Terminal) {
		tokens.push_back(currentToken);
		currentToken = stack.top();
		stack.pop();
	}
	delete currentToken;

	std::vector< Token * > params;

	size_t i = 1;
	while (i < tokens.size() && tokens[i]->getTerminal() != T_OBracket) {
		params.push_back(tokens[i]);
		++i;
	}
	tokens.erase(tokens.begin() + 1, tokens.begin() + i);
	deleteTmp(tokens);

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

	deleteTmp(tokens);

	stack.push(param);
}

void parser::R3(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;
	Token				  *currentToken = stack.top();
	stack.pop();

	while (!Token::IsKey(*currentToken)) {
		tokens.push_back(currentToken);
		currentToken = stack.top();
		stack.pop();
	}
	delete currentToken;

	std::vector< std::string > method;

	for (size_t i = 1; i < tokens.size(); ++i) {
		// if (Token::IsKey(*tokens[i])) {
		// 	break;
		// }
		method.push_back(tokens[i]->getValue());
	}
	deleteTmp(tokens);

	statement::DenyMethod *deniedMethod = new statement::DenyMethod(method);

	stack.push(deniedMethod);
}

void parser::R4(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;
	Token				  *currentToken = stack.top();
	stack.pop();

	while (!Token::IsKey(*currentToken)) {
		tokens.push_back(currentToken);
		currentToken = stack.top();
		stack.pop();
	}
	delete currentToken;

	const std::string		   value = tokens[1]->getValue();
	std::vector< std::string > errorCode;

	for (size_t i = 3; i < tokens.size(); ++i) {
		// if (Token::IsKey(*tokens[i])) {
		// 	delete tokens[i];
		// 	break;
		// }
		errorCode.push_back(tokens[i]->getValue());
	}
	deleteTmp(tokens);
	statement::ErrorPage *error = new statement::ErrorPage(errorCode, value);

	stack.push(error);
}

void parser::R5(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;
	Token				  *currentToken = stack.top();
	stack.pop();

	while (!Token::IsKey(*currentToken)) {
		tokens.push_back(currentToken);
		currentToken = stack.top();
		stack.pop();
	}
	delete currentToken;

	std::cout << "Tokens return : " << tokens.size() << std::endl;

	const std::string value = tokens[1]->getValue();

	const std::string code = tokens[3]->getValue();

	deleteTmp(tokens);
	std::cout << "ReturnParam : " << value << std::endl;
	statement::ReturnParam *returnParam =
		new statement::ReturnParam(code, value);

	stack.push(returnParam);
}

void parser::R6(std::stack< Token * > &stack)
{
	std::vector< Token * > tokens;
	Token				  *currentToken = stack.top();
	stack.pop();

	while (!Token::IsKey(*currentToken) ||
		   currentToken->getType() != S_Terminal) {
		tokens.push_back(currentToken);
		currentToken = stack.top();
		stack.pop();
	}
	delete currentToken;

	std::cout << "Tokens : " << tokens.size() << std::endl;
	for (size_t i = 0; i < tokens.size(); ++i) {
		std::cout << *tokens[i] << std::endl;
		statement::DenyMethod *deniedMethod =
			dynamic_cast< statement::DenyMethod * >(tokens[i]);
		if (tokens[i]->getType() == S_DenyMethod) {
			std::cout << "DenyMethod : " << *deniedMethod << std::endl;
		}
	}

	const std::string value = tokens[tokens.size() - 1]->getValue();

	std::vector< Token * > params;

	size_t i = 1;
	while (i < tokens.size() && tokens[i]->getTerminal() != T_OBracket) {
		params.push_back(tokens[i]);
		++i;
	}
	tokens.erase(tokens.begin() + 1, tokens.begin() + i);
	deleteTmp(tokens);

	statement::Location *location = new statement::Location(params, value);

	stack.push(location);
}