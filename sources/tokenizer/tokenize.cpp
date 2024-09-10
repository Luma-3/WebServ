/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:51:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 19:47:09 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "Parser.hpp"

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
	while (line[it] && isspace(line[it])) {
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

static void tokenize_line(string &line, vector<string> &tokens)
{
	size_t frontIT = 0;
	size_t backIT = 0;
	frontIT = skip_space(line, frontIT);
	while (frontIT < line.size()) {
		backIT = frontIT;
		if (!is_delimiter(line[frontIT])) {
			while (frontIT < line.size() && !is_delimiter(line[frontIT + 1]) &&
				   !isspace(line[frontIT + 1])) {
				frontIT++;
			}
		}
		tokens.push_back(take_token(frontIT, backIT, line));
		if (frontIT < line.size()) frontIT++;
		frontIT = skip_space(line, frontIT);
	}
}

vector<string> Tokenize(std::ifstream &file)
{
	vector<string> tokens;

	for (string line; std::getline(file, line);) {
		tokenize_line(line, tokens);
	}

	for (size_t i = 0; i < tokens.size(); i++) {
		cout << "token[" << i << "]: " << tokens[i] << endl;
	}

	return tokens;
}
