/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:01:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/04 16:04:46 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstdlib>
#include <string>

#include "lexer/Token.hpp"
#include "lexer/Lexer.hpp"

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
	if (value[value.size() - 1] != '/') {
		return (false);
	}
	return (true);
}

bool IsMethod(const string &value)
{
	static const int	size_key = 3;
	static const string key[size_key] = {"GET", "POST", "DELETE"};

	for (size_t i = 0; i < size_key; ++i) {
		if (value == key[i]) {
			return (true);
		}
	}
	return (false);
}

bool IsLogLevel(const string &value)
{
	static const int	size_key = 4;
	static const string key[size_key] = {"Debug", "Info", "Warning", "Error"};

	for (size_t i = 0; i < size_key; ++i) {
		if (value == key[i]) {
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
	if (value.size() != 3) {
		return (false);
	}
	if (value[0] >= '1' && value[0] <= '5') {
		return (true);
	}
	return (true);
}

bool IsIP(const string &value)
{
	size_t pos = 0;
	size_t count = 0;
	size_t size = value.size();

	while (pos < size) {
		if (!isdigit(value[pos])) {
			return (false);
		}
		int octet = atoi(value.substr(pos).c_str());
		if (octet < 0 || octet > MAXUCHAR) {
			return (false);
		}
		++count;
		pos += value.find('.', pos);
		if (pos == string::npos) {
			break;
		}
		++pos;
	}
	if (count != 4) {
		return (false);
	}
	return (true);
}

bool IsPort(const string &value)
{	
	if (!IsDigit(value) || value.size() > MAXPORTSIZE) {
		return (false);
	}
	int port = atoi(value.c_str());
	if (port < 0 || port > PORT_MAX) {
		return (false);
	}
	return (true);
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
	size_t size = value.size();
	for (size_t i = 0; i < size - 1; ++i) {
		if (!isdigit(value[i])) {
			return (false);
		}
	}
	char last = value[size - 1];
	if (isdigit(last)) {
		return (true);
	}
	last = static_cast<char>(toupper(last));
	if (last != 'K' && last != 'M' && last != 'G') {
		return (false);
	}
	return (true);
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