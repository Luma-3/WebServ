/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 14:05:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 14:37:11 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Action.hpp"

#include <iostream>
#include <stack>
#include <string>

#include "lexer/Token.hpp"
#include "parser/Parser.hpp"

Action::Action() {}

Action::Action(const Action &src) {}

Action::Action(ActionType type, int next_states, void (*rule_func)()) :
	_type(type),
	_next_state(next_states),
	_rule_func(rule_func)
{
}

Action &Action::operator=(const Action &src)
{
	if (this != &src) {}
	return *this;
}

void Action::Shift(Token *token, std::stack< Token * > &stack, Parser &parser)
{
	std::cout << "Shift: " << token->getValue() << std::endl;
	stack.push(token);

	parser.setState(_next_state);
}

void Action::Reduce(Token *token, std::stack< Token * > &stack, Parser &parser)
{
	// REDUCE FUNCTION
	std::cout << "Reduce: " << token->getValue() << std::endl;
	stack.push(token);

	_rule_func();

	// GOTO
	parser.setState(_next_state);
}

Action::~Action() {}

bool isValidParam(std::vector< Token * > &tokens)
{
	if (tokens.size() != 4) {
		std::cerr << "Error: Invalid return rule" << std::endl;
		return false;
	}
	return true;
}

bool isValidReturn(std::vector< Token * > &tokens)
{
	if (tokens.size() != 6) {
		std::cerr << "Error: Invalid return rule" << std::endl;
		return false;
		// TODO: Throw exception
	}
	return true;
}

void r1(std::stack< Token * > &stack)
{
	std::cout << "Rule 1" << std::endl;

	std::vector< Token * > tokens;

	do {
		tokens.push_back(stack.top());
		stack.pop();
	} while (tokens.back()->getType() != T_Key);

	if (tokens.back()->getValue() == "return" && !isValidReturn(tokens)) {
		exit(1);
	} else if (!isValidParam(tokens)) {
		exit(1);
	}

	
}