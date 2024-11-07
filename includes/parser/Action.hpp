/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 13:59:14 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/07 13:47:46 by Monsieur_Ca      ###   ########.fr       */
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
  private:
	int			  _state;
	Terminal_Type _terminal;

  public:
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
	int			  getState() const { return _state; }
	Terminal_Type getTerminal() const { return _terminal; }
};

} // namespace parser

#include "template/StringUtils.tpp"

#endif // ACTION_HPP