/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 13:59:14 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 10:53:09 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_HPP
#define ACTION_HPP

#include "lexer/Token.hpp"
#include "Parser.hpp"

namespace parser {

class Parser;

#define NB_ACTIONS 69

enum ActionType {
	SHIFT,
	REDUCE,
	ERROR
};

class Action
{
  private:
	ActionType _type;
	int		   _next_state;

	void (*_rule_func)(std::stack< Token * > &);

  public:
	Action();
	Action(ActionType type, int next_states,
		   void (*rule_func)(std::stack< Token * > &));
	Action(ActionType type, int next_states);
	Action(const Action &src);

	Action &operator=(const Action &src);

	void Shift(Token *token, std::stack< Token * > &stack,
			   Parser &parser) const;
	void Reduce(Token *token, std::stack< Token * > &stack,
				Parser &parser) const;
	int	 Execute(Token *token, std::stack< Token  *> &stack,
				 Parser &parser) const;
	~Action();
};

struct ActionEntry {
	Action		  action;
	int			  state;
	Terminal_Type terminal;
};

void R1_Server(std::stack< Token * > &stack);
void R2_Param(std::stack< Token * > &stack);
void R3_DoubleParam(std::stack< Token * > &stack);
void R4_ErrorPage(std::stack< Token * > &stack);
void R5_DenyMethod(std::stack< Token * > &stack);
void R6_Location(std::stack< Token * > &stack);

static const ActionEntry g_action[NB_ACTIONS] = {
	{Action(SHIFT, 1), 0, T_Server},
	{Action(SHIFT, 2), 1, T_OBracket},
	{Action(SHIFT, 21), 2, T_Location},
	{Action(REDUCE, 0, R1_Server), 2, T_CBracket},
	{Action(SHIFT, 3), 2, T_Port},
	{Action(SHIFT, 4), 2, T_Name},
	{Action(SHIFT, 41), 2, T_BodySize},
	{Action(SHIFT, 4), 2, T_Host},
	{Action(SHIFT, 5), 2, T_ErrorPage},
	{Action(SHIFT, 6), 2, T_Log},
	{Action(SHIFT, 7), 2, T_Root},
	{Action(SHIFT, 4), 2, T_Index},
	{Action(SHIFT, 8), 2, T_Return},
	{Action(SHIFT, 9), 2, T_AutoIndex},
	{Action(SHIFT, 10), 2, T_DenyMethod},
	{Action(SHIFT, 11), 3, T_Digits},
	{Action(SHIFT, 11), 4, T_Identifier},
	{Action(SHIFT, 12), 5, T_OSquareBracket},
	{Action(SHIFT, 13), 6, T_LogLevel},
	{Action(SHIFT, 11), 7, T_Path},
	{Action(SHIFT, 14), 8, T_Digits},
	{Action(SHIFT, 11), 9, T_Bool},
	{Action(SHIFT, 15), 10, T_OSquareBracket},
	{Action(REDUCE, 2, R2_Param), 11, T_Semi_Colon},
	{Action(SHIFT, 16), 12, T_Digits},
	{Action(SHIFT, 17), 13, T_Identifier},
	{Action(SHIFT, 18), 14, T_Identifier},
	{Action(SHIFT, 19), 15, T_Method},
	{Action(SHIFT, 39), 16, T_CSquareBracket},
	{Action(SHIFT, 12), 16, T_Comma},
	{Action(REDUCE, 2, R3_DoubleParam), 17, T_Semi_Colon},
	{Action(SHIFT, 20), 18, T_CBracket},
	{Action(SHIFT, 15), 18, T_Comma},
	{Action(REDUCE, 2, R4_ErrorPage), 19, T_Semi_Colon},
	{Action(REDUCE, 2, R5_DenyMethod), 20, T_Semi_Colon},
	{Action(SHIFT, 22), 21, T_Path},
	{Action(SHIFT, 23), 22, T_OBracket},
	{Action(REDUCE, 2, R6_Location), 23, T_CBracket},
	{Action(SHIFT, 42), 23, T_BodySize},
	{Action(SHIFT, 25), 23, T_ErrorPage},
	{Action(SHIFT, 26), 23, T_Root},
	{Action(SHIFT, 24), 23, T_Index},
	{Action(SHIFT, 27), 23, T_Return},
	{Action(SHIFT, 28), 23, T_AutoIndex},
	{Action(SHIFT, 29), 23, T_DenyMethod},
	{Action(SHIFT, 30), 24, T_Identifier},
	{Action(SHIFT, 31), 25, T_OSquareBracket},
	{Action(SHIFT, 30), 26, T_Path},
	{Action(SHIFT, 32), 27, T_Digits},
	{Action(SHIFT, 30), 28, T_Bool},
	{Action(SHIFT, 33), 29, T_OSquareBracket},
	{Action(REDUCE, 23, R2_Param), 30, T_Semi_Colon},
	{Action(SHIFT, 34), 31, T_Digits},
	{Action(SHIFT, 35), 32, T_Identifier},
	{Action(SHIFT, 36), 33, T_Method},
	{Action(SHIFT, 40), 34, T_CSquareBracket},
	{Action(SHIFT, 31), 34, T_Comma},
	{Action(REDUCE, 23, R3_DoubleParam), 35, T_Semi_Colon},
	{Action(SHIFT, 38), 36, T_CSquareBracket},
	{Action(SHIFT, 33), 36, T_Comma},
	{Action(REDUCE, 23, R4_ErrorPage), 37, T_Semi_Colon},
	{Action(REDUCE, 23, R5_DenyMethod), 38, T_Semi_Colon},
	{Action(SHIFT, 19), 39, T_Identifier},
	{Action(SHIFT, 37), 40, T_Identifier},
	{Action(SHIFT, 11), 41, T_Identifier},
	{Action(SHIFT, 11), 41, T_Digits},
	{Action(SHIFT, 30), 42, T_Identifier},
	{Action(SHIFT, 30), 42, T_Digits},
	{Action(ERROR, -1), -1, T_None},
};

} // namespace parser

#include "template/StringUtils.tpp"

#endif // ACTION_HPP