/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:51:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/20 16:42:49 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Lexer.hpp"

#include <iostream>
#include <string>

#include "lexer/Token.hpp"

using std::string;
using std::vector;

// Constructors

Lexer::Lexer() : _line(0), _col(0) {}

Lexer::Lexer(const Lexer &other) : _line(other._line), _col(other._col) {}

Lexer::Lexer(const char *file_path) : _line(0), _col(0)
{
	string file_path_str(file_path);
	size_t pos = file_path_str.find_last_of('.');
	if (pos == string::npos) {
		throw InvalidFileExtensionException(string("None"));
	}
	string extention = file_path_str.substr(pos + 1);
	if (extention != "conf") {
		throw InvalidFileExtensionException(extention);
	}

	_config_file.open(file_path);

	if (!_config_file.is_open()) {
		throw FileNotOpenException(file_path);
	}
}

// Operators

Lexer &Lexer::operator=(const Lexer &other)
{
	if (this != &other) {}
	return *this;
}

// Getters

const vector< Token * > &Lexer::getTokens() const
{
	return _tokens;
}

const std::ifstream &Lexer::getConfigFile() const
{
	return _config_file;
}

// Static Private Methods

bool Lexer::IsDelimiter(char c)
{
	return (c == ':' || c == ';' || c == '{' || c == '}' || c == '=' ||
			c == ',');
}

void Lexer::SkipSpace(const string &line, size_t &it)
{
	while (line[it] && (isspace(line[it]) != 0)) {
		it++;
	}
}

Token *Lexer::CreateToken(size_t frontIT, size_t backIT,
						  const string &line) const
{

	size_t size = frontIT - backIT + 1;
	string value(line, backIT, size);

	Terminal_Type type = Token::IdentifyTerminal(value);

	Token *token = new Token(value, type, _line, _col);

	return token;
}

void Lexer::TokenizeLine(const string &line, vector< Token * > &tokens)
{
	size_t frontIT = 0;
	size_t backIT = 0;

	SkipSpace(line, frontIT);
	while (frontIT < line.size()) {
		backIT = frontIT;
		_col = backIT;
		if (!IsDelimiter(line[frontIT])) {
			while (frontIT < line.size() && !IsDelimiter(line[frontIT + 1]) &&
				   (isspace(line[frontIT + 1]) == 0)) {
				frontIT++;
			}
		}
		tokens.push_back(CreateToken(frontIT, backIT, line));
		if (frontIT < line.size()) {
			frontIT++;
		}
		SkipSpace(line, frontIT);
	}
}

void Lexer::Tokenize()
{
	for (string line; std::getline(_config_file, line);) {
		_line++;
		TokenizeLine(line, _tokens);
	}
}

Lexer::~Lexer()
{
	for (size_t i = 0; i < _tokens.size(); i++) {
		delete _tokens[i];
	}
}