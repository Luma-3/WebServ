/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:51:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/14 14:43:08 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "parser/Parser.hpp"
#include "parser/Token.hpp"

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
	while (line[it] && (isspace(line[it]) != 0)) {
		it++;
	}
	return it;
}

static Token_Type identify_token(const string &value)
{
	string key[] = {"server",	  "listen", "server_name", "error_log",
					"acces_log",  "root",	"index",	   "autoindex",
					"host",		  "return", "location",	   "error_page",
					"deny_method"};

	for (size_t i = 0; i < 13; ++i) {
		if (value == key[i]) return (Key);
	}

	char c;
	c = value.c_str()[0];
	switch (c) {
		case ',':
			return (Comma);
		case ':':
			return (Colone);
		case ';':
			return (Semi_Colon);
		case '{':
			return (Bracket);
		case '}':
			return (Bracket);
		case '=':
			return (Equal);
		default:
			break;
	}
	return (Value);
}

static Token *create_token(size_t frontIT, size_t backIT, string &line)
{

	size_t size = frontIT - backIT + 1;
	string value(line, backIT, size);

	Token_Type type = identify_token(value);

	Token *token = new Token(value, type);

	return token;
}

static void tokenize_line(string &line, vector< Token * > &tokens)
{
	size_t frontIT = 0;
	size_t backIT = 0;
	frontIT = skip_space(line, frontIT);
	while (frontIT < line.size()) {
		backIT = frontIT;
		if (!is_delimiter(line[frontIT])) {
			while (frontIT < line.size() && !is_delimiter(line[frontIT + 1]) &&
				   (isspace(line[frontIT + 1]) == 0)) {
				frontIT++;
			}
		}
		tokens.push_back(create_token(frontIT, backIT, line));
		if (frontIT < line.size()) {
			frontIT++;
		}
		frontIT = skip_space(line, frontIT);
	}
}

void Parser::Tokenize()
{

	for (string line; std::getline(_config_file, line);) {
		tokenize_line(line, _tokens);
	}

	for (size_t i = 0; i < _tokens.size(); i++) {
		cout << "token[" << i << "]: " << _tokens[i] << endl;
	}
}
