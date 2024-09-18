/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:00:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 16:21:42 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Token.hpp"

#include <cstdlib>

using std::string;

Token::Token() : _type(S_Terminal), _terminal(T_None) {}

Token::Token(const Token &src) :
	_value(src._value),
	_type(src._type),
	_terminal(src._terminal)
{
}

Token &Token::operator=(const Token &src)
{
	if (this != &src) {
		this->_type = src._type;
		this->_value = src._value;
	}
	return (*this);
}

Token::Token(enum Token_Type type) : _type(type), _terminal(T_None) {}

Token::Token(const std::string &value, enum Terminal_Type terminal) :
	_value(value),
	_type(S_Terminal),
	_terminal(terminal)
{
}

Token::Token(enum Token_Type type, enum Terminal_Type terminal) :
	_type(type),
	_terminal(terminal)
{
}

Token::~Token() {}

enum Token_Type Token::getType() const
{
	return (_type);
}

enum Terminal_Type Token::getTerminal() const
{
	return (_terminal);
}

const std::string &Token::getValue() const
{
	return (_value);
}

bool IsPort(const string &value) // TODO: Verif negative number
{
	if (value.size() > 5) {
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

Terminal_Type Token::IdentifyTerminal(const string &value)
{
	static const int		 size_key = 12;
	static const IdentifyKey key[size_key] = {
		{T_Server,	   "server"	   },
		{T_Port,		 "port"	   },
		{T_Host,		 "host"	   },
		{T_ErroLog,	"error_log"  },
		{T_AccessLog,  "acces_log"	 },
		{T_Root,		 "root"	   },
		{T_Index,	  "index"		 },
		{T_AutoIndex,  "autoindex"	 },
		{T_Return,	   "return"	   },
		{T_Location,	 "location"   },
		{T_ErrorPage,  "error_page" },
		{T_DenyMethod, "deny_method"},
	};

	for (size_t i = 0; i < size_key; ++i) {
		if (value == key[i].key) {
			return (key[i].type);
		}
	}

	static const int		   size_regexs = 8;
	static const IdentifyRegex regexs[size_regexs] = {
		{T_Method,	   IsMethod   },
		{T_Bool,		 IsBool	   },
		{T_ErrorCode, IsErrorCode},
		{T_PortValue, IsPort		},
		{T_HostValue, IsHost		},
		{T_Path,		 IsPath	   },
		{T_FilePath,	 IsFilePath },
		{T_FileName,	 IsFileName },
	};

	for (size_t i = 0; i < size_regexs; ++i) {
		if (regexs[i].func(value)) {
			return (regexs[i].type);
		}
	}

	char c = *value.c_str();
	switch (c) {
		case ',':
			return (T_Comma);
		case ':':
			return (T_Colone);
		case ';':
			return (T_Semi_Colon);
		case '{':
			return (T_OBracket);
		case '}':
			return (T_CBracket);
		case '=':
			return (T_Equal);
		default:
			break;
	}
	return (T_Value);
}

bool Token::IsKey(const Token &token)
{
	Terminal_Type type = token.getTerminal();

	if (type <= T_AccessLog && type >= T_Server) {
		return (true);
	}
	return (false);
}
