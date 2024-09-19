/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:01:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/19 13:38:59 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <string>

#include "lexer/Token.hpp"

using std::string;

#define PORT_SIZE 5

bool IsPort(const string &value) // TODO: Verif negative number
{
	if (value.size() > PORT_SIZE) {
		return (false);
	}

	for (size_t i = 0; i < value.size(); ++i) {
		if (!isdigit(value[i])) {
			return (false);
		}
	}

	return (atoi(value.c_str()) <= PORT_MAX);
}

bool IsHost(const string &value)
{
	if (value[0] != '@') {
		return (false);
	}
	for (size_t i = 1; i < value.size(); ++i) {
		if (!isalnum(value[i]) && value[i] != '.' && value[i] != '-') {
			return (false);
		}
	}
	return (true);
}

bool IsErrorCode(const string &value)
{
	if (value.size() != 3) {
		return (false);
	}

	for (size_t i = 0; i < value.size(); ++i) {
		if (!isdigit(value[i])) {
			return (false);
		}
	}

	return (true);
}

bool IsFilePath(const string &value)
{
	if (value[0] != '/') {
		return (false);
	}
	for (size_t i = 1; i < value.size(); ++i) {
		if (!isalnum(value[i]) && value[i] != '.' && value[i] != '/' &&
			value[i] != '_' && value[i] != '-') {
			return (false);
		}
	}
	return (true);
}

bool IsPath(const string &value)
{
	if (value[0] != '/') {
		return (false);
	}
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

bool IsFileName(const string &value)
{
	for (size_t i = 0; i < value.size(); ++i) {
		if (!isalnum(value[i]) && value[i] != '.' && value[i] != '_' &&
			value[i] != '-') {
			return (false);
		}
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