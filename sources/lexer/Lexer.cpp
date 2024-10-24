/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:51:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/24 11:16:13 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Lexer.hpp"

#include <iostream>
#include <sys/stat.h>

#include "lexer/Token.hpp"

using std::queue;
using std::string;
using std::vector;

// Constructors

Lexer::Lexer(const char *file_path) : _line(0), _col(0)
{
	const string file_path_str(file_path);
	const size_t pos = file_path_str.find_last_of('.');
	if (pos == string::npos) {
		throw InvalidFileExtensionException(string("None"));
	}
	const string extention = file_path_str.substr(pos + 1);
	if (extention != "conf") {
		throw InvalidFileExtensionException(extention);
	}

	_config_file.open(file_path);

	struct stat buffer = {};
	if (!_config_file.is_open() ||
		(stat(file_path, &buffer) == 0 && buffer.st_mode & S_IFDIR)) {
		throw FileNotOpenException(file_path);
	}
}

// Static Private Methods

bool Lexer::IsDelimiter(char c)
{
	return (c == ';' || c == '{' || c == '}' || c == ',' || c == '[' ||
			c == ']' || c == '#' || c == ':');
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

	const size_t size = frontIT - backIT + 1;
	const string value(line, backIT, size);

	Terminal_Type term = Token::IdentifyTerminal(value);

	if (value[0] == '\"' || value[0] == '\'') {
		return (
			new Token(value.substr(1, value.size() - 2), term, _line, _col));
	}

	Token *token = new Token(value, term, _line, _col);

	return token;
}

void placeQuotes(const string &line, size_t &frontIT, char quoteType)
{
	frontIT++;
	while (line[frontIT] && line[frontIT] != quoteType) {
		frontIT++;
	}
}

void Lexer::TokenizeLine(const string &line, queue< Token * > &tokens)
{
	size_t frontIT = 0;
	size_t backIT = 0;

	SkipSpace(line, frontIT);
	while (frontIT < line.size()) {
		SkipSpace(line, frontIT);
		backIT = frontIT;
		_col = backIT;
		if (line[frontIT] == '\"' || line[frontIT] == '\'') {
			placeQuotes(line, frontIT, line[frontIT]);
		}
		else if (!IsDelimiter(line[frontIT])) {
			while (frontIT < line.size() && !IsDelimiter(line[frontIT + 1]) &&
				   (isspace(line[frontIT + 1]) == 0)) {
				frontIT++;
			}
		}
		if (line[frontIT] == '#') {
			break;
		}
		tokens.push(CreateToken(frontIT, backIT, line));
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
	while (!_tokens.empty()) {
		delete _tokens.front();
		_tokens.pop();
	}
}