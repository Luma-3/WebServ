/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 13:59:14 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/30 10:20:19 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_HPP
#define ACTION_HPP

#include "lexer/Token.hpp"
#include "Parser.hpp"
#include "template/Cast.tpp"

namespace parser {

class Parser;

#define NB_ACTIONS 69

enum ActionType {
	SHIFT,
	REDUCE,
	ERROR
};

enum ReduceRuleID {
	NONE,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7
};

class Action
{
  private:
	ActionType	 _type;
	int			 _next_state;
	ReduceRuleID _rule;

  public:
	Action();
	Action(ActionType type, int next_states, ReduceRuleID rule);
	Action(ActionType type, int next_states);
	Action(const Action &src);

	Action &operator=(const Action &src);

	void Shift(Token *token, std::stack< IParserToken * > &stack,
			   Parser &parser) const;
	void Reduce(Token *token, std::stack< IParserToken * > &stack,
				Parser &parser) const;
	int	 Execute(Token *token, std::stack< IParserToken	 *> &stack,
				 Parser &parser) const;
	~Action();
};

struct ActionEntry {
	int			  _state;
	Terminal_Type _terminal;

	ActionEntry(int state, Terminal_Type terminal) :
		_state(state),
		_terminal(terminal)
	{
	}

	bool operator<(const ActionEntry &rhs) const
	{
		if (_state < rhs._state) {
			return true;
		}
		if (_state > rhs._state) {
			return false;
		}
		return _terminal < rhs._terminal;
	}

	bool operator==(const ActionEntry &rhs) const
	{
		return _state == rhs._state && _terminal == rhs._terminal;
	}
};


} // namespace parser

#include "template/StringUtils.tpp"// void R1_Server(std::stack< IParserToken * > &stack);
// void R2_Param(std::stack< IParserToken * > &stack);
// void R3_DoubleParam(std::stack< IParserToken * > &stack);
// void R4_ErrorPage(std::stack< IParserToken * > &stack);
// void R5_DenyMethod(std::stack< IParserToken * > &stack);
// void R6_Location(std::stack< IParserToken * > &stack);

#endif // ACTION_HPP