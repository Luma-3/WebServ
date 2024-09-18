/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:03:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 19:08:52 by jbrousse         ###   ########.fr       */
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
};

#endif // PARSER_HPP