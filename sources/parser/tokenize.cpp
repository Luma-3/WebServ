/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:51:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/11 15:44:03 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "parser/Parser.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

static bool is_delimiter(char c)
{
	return (c == ':' || c == ';' || c == '{' || c == '}' || c == '=' ||
			c == ',');
}

static size_t skip_space(string &line, size_t it)
{
	while (line[it] && (isspace(line[it]) == 0)) {
		it++;
	}
	return it;
}

static string take_token(size_t frontIT, size_t backIT, string &line)
{
	size_t size = frontIT - backIT + 1;
	string token(line, backIT, size);

	return token;
}

static void tokenize_line(string &line, vector< string > &tokens)
{
	size_t frontIT = 0;
	size_t backIT = 0;
	frontIT = skip_space(line, frontIT);
	while (frontIT < line.size()) {
		backIT = frontIT;
		if (!is_delimiter(line[frontIT])) {
			while (frontIT < line.size() && !is_delimiter(line[frontIT + 1]) &&
				   (isspace(line[frontIT + 1]) != 0)) {
				frontIT++;
			}
		}
		tokens.push_back(take_token(frontIT, backIT, line));
		if (frontIT < line.size()) {
			frontIT++;
		}
		frontIT = skip_space(line, frontIT);
	}
}

void Parser::Tokenize()
{
	vector< string > tokens;

	string line;
	while (std::getline(_config_file, line)) {
		tokenize_line(line, tokens);
	}
}
