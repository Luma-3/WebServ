/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:03:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/11 15:49:10 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "Token.hpp"

std::vector< std::string > Tokenize(std::ifstream &file);

class Parser
{
  private:
	std::ifstream			   _config_file;
	std::vector< std::string > _tokens;
	int						   _status;
	std::stack< Token >		   _parse_stack;

	// static bool isErrorCode(std::string &val);
	// static bool isPort(std::string &val);
	// static bool isBool(std::string &val);

  public:
	Parser();
	Parser(const Parser &src);
	Parser(const std::string &file_path);
	~Parser();

	void Parse();
	void Tokenize();

	Parser &operator=(const Parser &src);
};

#endif // PARSER_HPP