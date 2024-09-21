/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:03:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/21 14:17:21 by jbrousse         ###   ########.fr       */
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
#include "lexer/Token.hpp"

namespace parser {
class Parser
{
  private:
	std::vector< Token * > _tokens;
	int					   _status;
	std::stack< Token * >  _parse_stack;

	// std::map< std::pair< int, std::string >, Action > _config;

  public:
	Parser();
	Parser(const Parser &src);
	Parser(const std::vector< Token * > &tokens);
	~Parser();

	void setState(int state);

	void Parse();

	const std::stack< Token * > &getParseStack() const;

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
};
} // namespace parser

#endif // PARSER_HPP