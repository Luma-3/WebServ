/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:01:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 10:44:30 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstdlib>
#include <string>

#include "lexer/Token.hpp"

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

bool IsPort(const string &value)
{
	if (value.size() > 5) {
		return (false);
	}
	int integer = atoi(value.c_str());
	if (integer < 0 || integer > PORT_MAX) {
		return (false);
	}
	return (true);
}

bool IsHost(const string &value)
{
	if (value.size() > 255 || value.size() == 0) {
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
	last = toupper(last);
	if (last != 'K' && last != 'M' && last != 'G') {
		return (false);
	}
	return (true);
}