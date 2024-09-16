/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:03:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/16 10:31:59 by jbrousse         ###   ########.fr       */
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

#include "Token.hpp"

// void initActionTable(std::map< std::pair< int, std::string >, Action >
// &config)
// {
// 	config[std::make_pair(0, "server")] = Action(SHIFT, 1);
// 	config[std::make_pair(1, "{")] = Action(SHIFT, 2);
// 	config[std::make_pair(2, "port")] = Action(SHIFT, 3);
// 	config[std::make_pair(3, "=")] = Action(SHIFT, 4);
// 	config[std::make_pair(4, "")] = Action(SHIFT, 5);
// 	config[std::make_pair(5, ";")] = Action(REDUCE, -1, );
// }

class Parser
{
  private:
	std::ifstream		   _config_file;
	std::vector< Token * > _tokens;
	int					   _status;
	std::stack< Token * >  _parse_stack;

	// std::map< std::pair< int, std::string >, Action > _config;

  public:
	Parser();
	Parser(const Parser &src);
	Parser(const char *file_path);
	~Parser();

	void Parse();
	void Tokenize();

	const std::vector< Token * > &getTokens() const;

	Parser &operator=(const Parser &src);
};

#endif // PARSER_HPP