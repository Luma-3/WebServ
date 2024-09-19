/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:00:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/19 11:27:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Token.hpp"

#include <iomanip>

using std::string;

// Constructors

Token::Token() : _type(S_Terminal), _terminal(T_None), _line(0), _col(0) {}

Token::Token(const Token &src) :
	_value(src._value),
	_type(src._type),
	_terminal(src._terminal)
{
}
Token::Token(enum Token_Type type) : _type(type), _terminal(T_None) {}

Token::Token(const std::string &value, enum Terminal_Type terminal, int line,
			 int col) :
	_value(value),
	_type(S_Terminal),
	_terminal(terminal),
	_line(line),
	_col(col)
{
}

Token::Token(enum Token_Type type, enum Terminal_Type terminal) :
	_type(type),
	_terminal(terminal)
{
}

// Operators

Token &Token::operator=(const Token &src)
{
	if (this != &src) {
		this->_type = src._type;
		this->_value = src._value;
	}
	return (*this);
}

// Getters

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

int Token::getLine() const
{
	return (_line);
}

int Token::getCol() const
{
	return (_col);
}

// Static functions

bool Token::IsKey(const Token &token)
{
	Terminal_Type type = token.getTerminal();

	if (type <= T_AccessLog && type >= T_Server) {
		return (true);
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

// Destructor

Token::~Token() {}

// Print Operator

std::ostream &operator<<(std::ostream &os, const Token &token)
{
	os << std::setw(15) << std::left << token.getValue() << " | ";
	os << std::left << "term: " << std::setw(5) << token.getTerminal() << " | ";
	os << std::left << "line: " << std::setw(5) << token.getLine() << " | ";
	os << std::left << "col: " << std::setw(7) << token.getCol();

	return (os);
}