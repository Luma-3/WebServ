/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:03:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/20 13:31:24 by jbrousse         ###   ########.fr       */
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

	static std::map< ActionEntry, Action > createActionMap();

  public:
	Parser();
	~Parser();

	Parser(Lexer *lexer);

	std::stack< IParserToken * > &getParseStack() { return _parse_stack; };
	const Lexer					 &getLexer() const { return *_lexer; };
	const int					 &getState() const { return _status; };
	const VirtualServer			 *getCurrent() const { return _current; };
	const std::vector< VirtualServer * > &getServers() const
	{
		return _servers;
	};

	void setState(int state) { _status = state; };
	void setParseStack(std::stack< IParserToken * > stack)
	{
		_parse_stack = stack;
	};

	Action		findAction(int state, Terminal_Type terminal);
	std::string findExpected(int state);
	void		Parse();

	void R1_Server();
	void R2_Param();

	static void R3_CaseListen(std::stack< Token * > &tokens,
							  const std::string &value1, std::string &value2,
							  size_t &tmp_col, size_t &tmp_line);
	void		R3_DoubleParam();
	void		R4_ErrorPage();
	void		R5_DenyMethod();
	void		R6_Location();
	void		R7_CGI();

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