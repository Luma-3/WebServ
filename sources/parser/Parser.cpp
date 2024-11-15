/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:28:51 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/15 22:20:52 by jbrousse         ###   ########.fr       */
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
using parser::ActionEntry;
using parser::Parser;

Parser::Parser() :
	_status(0),
	_current(new VirtualServer),
	_actions(createActionMap())
{
}

Parser::Parser(Lexer *lexer) :
	_lexer(lexer),
	_status(0),
	_current(new VirtualServer),
	_actions(createActionMap())
{
}

std::map< parser::ActionEntry, Action > Parser::createActionMap()
{
	std::map< ActionEntry, Action > actions;

	// NOLINTBEGIN(*-magic-numbers) - Magic numbers are used to define the
	// state and terminal
	actions[ActionEntry(0, T_Server)] = Action(SHIFT, 1);
	actions[ActionEntry(0, T_Log)] = Action(SHIFT, 6);
	actions[ActionEntry(1, T_OBracket)] = Action(SHIFT, 2);

	actions[ActionEntry(2, T_Location)] = Action(SHIFT, 21);
	actions[ActionEntry(2, T_CBracket)] = Action(REDUCE, 0, R1);
	actions[ActionEntry(2, T_Listen)] = Action(SHIFT, 3);
	actions[ActionEntry(2, T_Hostname)] = Action(SHIFT, 4);
	actions[ActionEntry(2, T_BodySize)] = Action(SHIFT, 41);
	actions[ActionEntry(2, T_ErrorPage)] = Action(SHIFT, 5);
	actions[ActionEntry(2, T_Root)] = Action(SHIFT, 7);
	actions[ActionEntry(2, T_Index)] = Action(SHIFT, 4);
	actions[ActionEntry(2, T_Return)] = Action(SHIFT, 8);
	actions[ActionEntry(2, T_AutoIndex)] = Action(SHIFT, 9);
	actions[ActionEntry(2, T_DenyMethod)] = Action(SHIFT, 10);
	actions[ActionEntry(2, T_CGI)] = Action(SHIFT, 46);
	actions[ActionEntry(2, T_UploadDir)] = Action(SHIFT, 7);

	actions[ActionEntry(3, T_Identifier)] = Action(SHIFT, 43);
	actions[ActionEntry(4, T_Identifier)] = Action(SHIFT, 11);
	actions[ActionEntry(5, T_OSquareBracket)] = Action(SHIFT, 12);
	actions[ActionEntry(6, T_LogLevel)] = Action(SHIFT, 13);
	actions[ActionEntry(7, T_Path)] = Action(SHIFT, 11);
	actions[ActionEntry(8, T_Digits)] = Action(SHIFT, 14);
	actions[ActionEntry(9, T_Bool)] = Action(SHIFT, 11);
	actions[ActionEntry(10, T_OSquareBracket)] = Action(SHIFT, 15);
	actions[ActionEntry(11, T_SemiColon)] = Action(REDUCE, 2, R2);
	actions[ActionEntry(12, T_Digits)] = Action(SHIFT, 16);
	actions[ActionEntry(13, T_Identifier)] = Action(SHIFT, 45);
	actions[ActionEntry(14, T_Identifier)] = Action(SHIFT, 17);
	actions[ActionEntry(14, T_SemiColon)] = Action(REDUCE, 2, R3);
	actions[ActionEntry(15, T_Method)] = Action(SHIFT, 18);
	actions[ActionEntry(16, T_CSquareBracket)] = Action(SHIFT, 39);
	actions[ActionEntry(16, T_Comma)] = Action(SHIFT, 12);
	actions[ActionEntry(17, T_SemiColon)] = Action(REDUCE, 2, R3);
	actions[ActionEntry(18, T_CSquareBracket)] = Action(SHIFT, 20);
	actions[ActionEntry(18, T_Comma)] = Action(SHIFT, 15);
	actions[ActionEntry(19, T_SemiColon)] = Action(REDUCE, 2, R4);
	actions[ActionEntry(20, T_SemiColon)] = Action(REDUCE, 2, R5);
	actions[ActionEntry(21, T_Path)] = Action(SHIFT, 22);
	actions[ActionEntry(22, T_OBracket)] = Action(SHIFT, 23);
	actions[ActionEntry(23, T_CBracket)] = Action(REDUCE, 2, R6);

	actions[ActionEntry(23, T_BodySize)] = Action(SHIFT, 42);
	actions[ActionEntry(23, T_ErrorPage)] = Action(SHIFT, 25);
	actions[ActionEntry(23, T_Root)] = Action(SHIFT, 26);
	actions[ActionEntry(23, T_Index)] = Action(SHIFT, 24);
	actions[ActionEntry(23, T_Return)] = Action(SHIFT, 27);
	actions[ActionEntry(23, T_AutoIndex)] = Action(SHIFT, 28);
	actions[ActionEntry(23, T_DenyMethod)] = Action(SHIFT, 29);
	actions[ActionEntry(23, T_CGI)] = Action(SHIFT, 49);
	actions[ActionEntry(23, T_UploadDir)] = Action(SHIFT, 26);

	actions[ActionEntry(24, T_Identifier)] = Action(SHIFT, 30);
	actions[ActionEntry(25, T_OSquareBracket)] = Action(SHIFT, 31);
	actions[ActionEntry(26, T_Path)] = Action(SHIFT, 30);
	actions[ActionEntry(27, T_Digits)] = Action(SHIFT, 32);
	actions[ActionEntry(28, T_Bool)] = Action(SHIFT, 30);
	actions[ActionEntry(29, T_OSquareBracket)] = Action(SHIFT, 33);
	actions[ActionEntry(30, T_SemiColon)] = Action(REDUCE, 23, R2);
	actions[ActionEntry(31, T_Digits)] = Action(SHIFT, 34);
	actions[ActionEntry(32, T_Identifier)] = Action(SHIFT, 35);
	actions[ActionEntry(32, T_SemiColon)] = Action(REDUCE, 23, R3);
	actions[ActionEntry(33, T_Method)] = Action(SHIFT, 36);
	actions[ActionEntry(34, T_CSquareBracket)] = Action(SHIFT, 40);
	actions[ActionEntry(34, T_Comma)] = Action(SHIFT, 31);
	actions[ActionEntry(35, T_SemiColon)] = Action(REDUCE, 23, R3);
	actions[ActionEntry(36, T_CSquareBracket)] = Action(SHIFT, 38);
	actions[ActionEntry(36, T_Comma)] = Action(SHIFT, 33);
	actions[ActionEntry(37, T_SemiColon)] = Action(REDUCE, 23, R4);
	actions[ActionEntry(38, T_SemiColon)] = Action(REDUCE, 23, R5);
	actions[ActionEntry(39, T_Identifier)] = Action(SHIFT, 19);
	actions[ActionEntry(40, T_Identifier)] = Action(SHIFT, 37);
	actions[ActionEntry(41, T_Identifier)] = Action(SHIFT, 11);
	actions[ActionEntry(41, T_Digits)] = Action(SHIFT, 11);
	actions[ActionEntry(42, T_Identifier)] = Action(SHIFT, 30);
	actions[ActionEntry(42, T_Digits)] = Action(SHIFT, 30);
	actions[ActionEntry(43, T_Colon)] = Action(SHIFT, 44);
	actions[ActionEntry(44, T_Digits)] = Action(SHIFT, 17);
	actions[ActionEntry(45, T_SemiColon)] = Action(REDUCE, 0, R3);
	actions[ActionEntry(46, T_CGIExtension)] = Action(SHIFT, 47);
	actions[ActionEntry(47, T_Identifier)] = Action(SHIFT, 48);
	actions[ActionEntry(48, T_SemiColon)] = Action(REDUCE, 2, R7);
	actions[ActionEntry(49, T_CGIExtension)] = Action(SHIFT, 50);
	actions[ActionEntry(50, T_Identifier)] = Action(SHIFT, 51);
	actions[ActionEntry(51, T_SemiColon)] = Action(REDUCE, 23, R7);
	// NOLINTEND(*-magic-numbers)

	return actions;
}

Action Parser::findAction(int state, Terminal_Type terminal)
{
	const ActionEntry entry(state, terminal);

	if (_actions.find(entry) != _actions.end()) {
		return _actions.at(entry);
	}
	return Action(ERROR, 0);
}

std::string Parser::findExpected(int state)
{
	std::set< Terminal_Type > expected;

	for (std::map< ActionEntry, Action >::const_iterator it = _actions.begin();
		 it != _actions.end(); ++it) {
		if (it->first.getState() == state) {
			expected.insert(it->first.getTerminal());
		}
	}
	std::string expected_values;
	for (std::set< Terminal_Type >::const_iterator it = expected.begin();
		 it != expected.end(); ++it) {
		if (Token::IsKey(Token(*it))) {
			expected_values +=
				"Valid key such as <index, root, listen, etc...>";
			break;
		}
		expected_values += "`" + Token::TerminalToString(*it) + "` ";
	}

	return expected_values;
}

void Parser::Parse()
{
	std::queue< Token * > &tokens = _lexer->getTokens();

	while (!tokens.empty()) {
		Token		*token = tokens.front();
		const Action action = findAction(_status, token->getTerminal());

		tokens.pop();
		if (action.Execute(token, _parse_stack, *this) == ERROR) {
			const std::string value = token->getKey();
			const size_t	  col = token->getCol();
			const size_t	  line = token->getLine();
			delete token;
			throw UnexpectedTokenException(col, line, value,
										   findExpected(_status));
		}
	}
}

Parser::~Parser()
{
	while (!_parse_stack.empty()) {
		delete _parse_stack.top();
		_parse_stack.pop();
	}
	delete _current;
}
