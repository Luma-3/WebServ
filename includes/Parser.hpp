/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:03:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 16:06:27 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> Tokenize(std::ifstream &file);

class Parser
{
  private:
	std::ifstream _config_file;

	bool isErrorCode(std::string &val);
	bool isPort(std::string &val);
	bool isBool(std::string &val);

  public:
	Parser(std::string file_path);
	~Parser();
};

#endif // PARSER_HPP