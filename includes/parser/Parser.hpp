/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:03:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/24 13:53:40 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "Action.hpp"
#include "color.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/Location.hpp"
#include "server/VirtualServer.hpp"

namespace parser {

struct ActionEntry;
class Action;
class Parser
{
  private:
	Lexer						  *_lexer;
	int							   _status;
	std::stack< IParserToken * >   _parse_stack;
	VirtualServer				  *_current;
	std::vector< VirtualServer * > _servers;

	const std::map< ActionEntry, Action > _actions;

	std::map< ActionEntry, Action > createActionMap();

  public:
	Parser();
	Parser(const Parser &src);
	Parser &operator=(const Parser &src);
	~Parser();

	Parser(Lexer *lexer);

	std::stack< IParserToken * > &getParseStack() { return _parse_stack; };
	const Lexer					 &getLexer() const { return *_lexer; };
	const int					 &getState() const { return _status; };
	const std::vector< VirtualServer * > &getServers() const
	{
		return _servers;
	};

	void setState(int state) { _status = state; };

	Action		findAction(int state, Terminal_Type terminal);
	std::string findExpected(int state);
	void		Parse();

	void R1_Server();
	void R2_Param();
	void R3_DoubleParam();
	void R4_ErrorPage();
	void R5_DenyMethod();
	void R6_Location();

	// EXCEPTIONS

	class UnexpectedTokenException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		UnexpectedTokenException(size_t _col, size_t _line,
								 const std::string &value,
								 const std::string &expected);
		UnexpectedTokenException(const UnexpectedTokenException &src);

		UnexpectedTokenException &
		operator=(const UnexpectedTokenException &src);

		virtual const char *what() const throw();

		virtual ~UnexpectedTokenException() throw();
	};

	class MissingParamException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		MissingParamException(const std::string &value);
		MissingParamException(const MissingParamException &src);

		MissingParamException &operator=(const MissingParamException &src);

		virtual const char *what() const throw();

		virtual ~MissingParamException() throw();
	};
};

} // namespace parser

#endif // PARSER_HPP