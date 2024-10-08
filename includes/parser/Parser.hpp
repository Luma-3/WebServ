/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:03:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 11:54:59 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

#include "Action.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"

namespace parser {
class Parser
{
  private:
	Lexer				 *_lexer;
	int					  _status;
	std::stack< Token * > _parse_stack;

	// std::map< std::pair< int, std::string >, Action > _config;

  public:
	Parser();
	Parser(const Parser &src);
	Parser(Lexer *lexer);
	~Parser();

	void setState(int state);

	void Parse();

	std::stack< Token * > &getParseStack();
	const Lexer			  &getLexer() const;
	const int			  &getState() const;

	Parser &operator=(const Parser &src);

	class InvalidTokenException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		InvalidTokenException(size_t _col, size_t _line,
							  const std::string &value,
							  const std::string &expected);
		InvalidTokenException(const InvalidTokenException &src);

		InvalidTokenException &operator=(const InvalidTokenException &src);

		virtual const char *what() const throw();

		virtual ~InvalidTokenException() throw();
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