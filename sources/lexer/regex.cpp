/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:01:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/15 11:46:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstdlib>
#include <string>

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"

#define PORT_MAX 65535

using std::string;

bool IsDigit(const string &value)
{
	for (size_t i = 0; i < value.size(); ++i) {
		if (!isdigit(value[i])) {
			return (false);
		}
	}
	return (true);
}

bool IsPath(const string &value)
{
	for (size_t i = 1; i < value.size(); ++i) {
		if (!isalnum(value[i]) && value[i] != '.' && value[i] != '/' &&
			value[i] != '_' && value[i] != '-') {
			return (false);
		}
	}
	return (value[value.size() - 1] == '/');
}

bool IsMethod(const string &value)
{
	static const int	size_key = 3;
	static const string key[size_key] = {"GET", "POST", "DELETE"};

	std::string upper;
	for (size_t i = 0; i < value.size(); ++i) {
		upper += static_cast< char >(toupper(value[i]));
	}

	for (size_t i = 0; i < size_key; ++i) {
		if (upper == key[i]) {
			return (true);
		}
	}
	return (false);
}

bool IsLogLevel(const string &value)
{
	static const int	size_key = 4;
	static const string key[size_key] = {"debug", "info", "warning", "error"};

	std::string lower;

	for (size_t i = 0; i < value.size(); ++i) {
		lower += static_cast< char >(tolower(value[i]));
	}

	for (size_t i = 0; i < size_key; ++i) {
		if (lower == key[i]) {
			return (true);
		}
	}
	return (false);
}

bool IsBool(const string &value)
{
	static const int	size_key = 2;
	static const string key[size_key] = {"on", "off"};

	for (size_t i = 0; i < size_key; ++i) {
		if (value == key[i]) {
			return (true);
		}
	}
	return (false);
}

bool IsErrorCode(const string &value)
{
	if (value.size() != 3 || !IsDigit(value)) {
		return (false);
	}
	return (value[0] >= '1' && value[0] <= '5');
}

bool IsIP(const string &value)
{
	const size_t size = value.size();
	std::string	 octet;
	size_t		 pos = 0;
	size_t		 dot_pos = 0;
	size_t		 count = 0;

	while (count < 4) {
		dot_pos = value.find('.', pos);
		if (dot_pos == string::npos) {
			dot_pos = size;
		}
		octet = value.substr(pos, dot_pos - pos);
		if (!IsDigit(octet)) {
			return (false);
		}
		const int octet_int = atoi(octet.c_str());
		if (octet_int < 0 || octet_int > MAXUCHAR) {
			return (false);
		}
		pos = dot_pos + 1;
		if (pos >= size) {
			break;
		}
		++count;
	}
	return (count == 3);
}

bool IsPort(const string &value)
{
	if (!IsDigit(value) || value.size() > MAXPORTSIZE) {
		return (false);
	}
	const int port = atoi(value.c_str());
	return (!(port < 0 && port > PORT_MAX));
}

bool IsHostname(const string &value)
{
	if (value.size() > MAXHOSTNAMESIZE || value.empty()) {
		return (false);
	}
	for (size_t i = 0; i < value.size(); ++i) {
		if (!isalnum(value[i]) && value[i] != '.' && value[i] != '-') {
			return (false);
		}
	}
	return (true);
}

bool IsBodySize(const string &value)
{
	const size_t size = value.size();
	for (size_t i = 0; i < size - 1; ++i) {
		if (!isdigit(value[i])) {
			return (false);
		}
	}
	char last = value[size - 1];
	if (isdigit(last)) {
		return (true);
	}
	last = static_cast< char >(toupper(last));
	return (!(last != 'K' && last != 'M' && last != 'G'));
}

bool IsCGIExtension(const string &value)
{
	static const int	size_key = 2;
	static const string key[size_key] = {"php", "py"};

	for (size_t i = 0; i < size_key; ++i) {
		if (value == key[i]) {
			return (true);
		}
	}
	return (false);
}