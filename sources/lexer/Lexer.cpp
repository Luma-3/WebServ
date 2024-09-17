/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:51:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 14:13:55 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Lexer.hpp"

#include <iostream>
#include <string>

#include "lexer/Token.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

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

Token *Lexer::CreateToken(size_t frontIT, size_t backIT, const string &line)
{

	size_t size = frontIT - backIT + 1;
	string value(line, backIT, size);

	Token_Type type = Token::IdentifyToken(value);

	Token *token = new Token(value, type);

	return token;
}

void Lexer::TokenizeLine(const string &line, vector< Token * > &tokens)
{
	size_t frontIT = 0;
	size_t backIT = 0;

	SkipSpace(line, frontIT);
	while (frontIT < line.size()) {
		backIT = frontIT;
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

const vector< Token * > &Lexer::getTokens() const
{
	return _tokens;
}

const std::ifstream &Lexer::getConfigFile() const
{
	return _config_file;
}

void Lexer::Tokenize()
{

	for (string line; std::getline(_config_file, line);) {
		TokenizeLine(line, _tokens);
	}

	for (size_t i = 0; i < _tokens.size(); i++) {
		cout << "token[" << i << "]: " << _tokens[i]->getValue() << endl;
	}
}

Lexer::Lexer() {}

Lexer::~Lexer() {}

Lexer::Lexer(const Lexer &other)
{
	*this = other;
}

Lexer::Lexer(const char *file_path)
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

Lexer &Lexer::operator=(const Lexer &other)
{
	if (this != &other) {}
	return *this;
}
