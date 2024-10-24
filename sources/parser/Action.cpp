/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 14:05:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/24 13:54:04 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Action.hpp"

using parser::Action;

Action::Action() : _type(ERROR), _next_state(-1), _rule(NONE) {}

Action::Action(const Action &src) :
	_type(src._type),
	_next_state(src._next_state),
	_rule(src._rule)
{
}

Action::Action(ActionType type, int next_states, ReduceRuleID rule) :
	_type(type),
	_next_state(next_states),
	_rule(rule)
{
}

Action::Action(ActionType type, int next_states) :
	_type(type),
	_next_state(next_states),
	_rule(NONE)
{
}

Action &Action::operator=(const Action &src)
{
	if (this != &src) {
		_type = src._type;
		_next_state = src._next_state;
		_rule = src._rule;
	}
	return *this;
}

void Action::Shift(Token *token, std::stack< IParserToken * > &stack,
				   Parser &parser) const
{
	Token *currentToken = token;

	stack.push(currentToken);
	parser.setState(_next_state);
}

void Action::Reduce(Token *token, std::stack< IParserToken * > &stack,
					Parser &parser) const
{
	Shift(token, stack, parser);
	switch (_rule) {
		case R1:
			parser.R1_Server();
			break;
		case R2:
			parser.R2_Param();
			break;
		case R3:
			parser.R3_DoubleParam();
			break;
		case R4:
			parser.R4_ErrorPage();
			break;
		case R5:
			parser.R5_DenyMethod();
			break;
		case R6:
			parser.R6_Location();
			break;
		default:
			break;
	}
}

int Action::Execute(Token *token, std::stack< IParserToken * > &stack,
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