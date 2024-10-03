/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:00:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/03 10:31:03 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Token.hpp"

#include <iomanip>
#include <string>

// Constructors

Token::Token() : _type(S_Terminal), _terminal(T_None), _line(0), _col(0) {}

Token::Token(const Token &src) :
	_value(src._value),
	_type(src._type),
	_terminal(src._terminal),
	_line(src._line),
	_col(src._col)
{
}
Token::Token(enum Token_Type type) :
	_type(type),
	_terminal(T_None),
	_line(0),
	_col(0)
{
}

Token::Token(const std::string &value, enum Terminal_Type terminal, size_t line,
			 size_t col) :
	_value(value),
	_type(S_Terminal),
	_terminal(terminal),
	_line(line),
	_col(col)
{
}

Token::Token(enum Token_Type type, enum Terminal_Type terminal) :
	_type(type),
	_terminal(terminal),
	_line(0),
	_col(0)
{
}

// Operators

Token &Token::operator=(const Token &src)
{
	if (this != &src) {
		this->_type = src._type;
		this->_value = src._value;
		this->_terminal = src._terminal;
		this->_line = src._line;
		this->_col = src._col;
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

size_t Token::getLine() const
{
	return (_line);
}

size_t Token::getCol() const
{
	return (_col);
}

// Static functions

bool Token::IsKey(const Token &token)
{
	const Terminal_Type type = token.getTerminal();

	if (type <= T_AccessLog && type >= T_Server) {
		return (true);
	}
	return (false);
}

Terminal_Type Token::IdentifyTerminal(const std::string &value)
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

	static const int		   size_regexs = 7;
	static const IdentifyRegex regexs[size_regexs] = {
		{T_Method,	   IsMethod   },
		{T_Bool,		 IsBool	   },
		{T_ErrorCode, IsErrorCode},
		{T_PortValue, IsPort		},
		{T_HostValue, IsHost		},
		{T_Path,		 IsPath	   },
		{T_File,		 IsFile	   },
	};

	for (size_t i = 0; i < size_regexs; ++i) {
		if (regexs[i].func(value)) {
			return (regexs[i].type);
		}
	}

	const char c = *value.c_str();
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

const std::string &Token::TerminalTypeToString(enum Terminal_Type type)
{
	static const int		 size_key = 26;
	static const IdentifyKey key[size_key] = {
		{T_Server,	   "server"					   },
		{T_Port,		 "port"					   },
		{T_Host,		 "host"					   },
		{T_ErroLog,	"error_log"				   },
		{T_AccessLog,  "acces_log"					 },
		{T_Root,		 "root"					   },
		{T_Index,	  "index"						 },
		{T_AutoIndex,  "autoindex"					 },
		{T_Return,	   "return"					   },
		{T_Location,	 "location"				   },
		{T_ErrorPage,  "error_page"				  },
		{T_DenyMethod, "deny_method"				},
		{T_Comma,	  ","						  },
		{T_Semi_Colon, ";"						  },
		{T_CBracket,	 "}"						  },
		{T_OBracket,	 "{"						  },
		{T_Colone,	   ":"						  },
		{T_Equal,	  "="						  },
		{T_Value,	  "`value`"					   },
		{T_ErrorCode,  "`000`"						 },
		{T_Method,	   "`GET` or `POST` or `DELETE`"},
		{T_File,		 "`file`"					 },
		{T_PortValue,  "`00000`"					   },
		{T_HostValue,  "`host_value`"				},
		{T_Path,		 "`path`"					 },
		{T_Bool,		 "`on` or `off`"				},
	};

	for (size_t i = 0; i < size_key; ++i) {
		if (type == key[i].type) {
			return (key[i].key);
		}
	}
	return (key[0].key);
}

// Destructor

Token::~Token() {}

// Exceptions

Token::InvalidTokenException::InvalidTokenException() {}
Token::InvalidTokenException::InvalidTokenException(
	const InvalidTokenException &src)
{
	*this = src;
}

Token::InvalidTokenException &
Token::InvalidTokenException::operator=(const InvalidTokenException &src)
{
	(void)src;
	return (*this);
}

Token::InvalidTokenException::~InvalidTokenException() throw() {}

const char *Token::InvalidTokenException::what() const throw()
{
	return ("Invalid Token");
}

// Print Operator

#define SIZE_FORMAT 15

std::ostream &operator<<(std::ostream &os, const Token &token)
{
	const std::string &value = token.getValue();

	os << std::setw(SIZE_FORMAT) << std::left
	   << ((value.size() <= SIZE_FORMAT)
			   ? value
			   : value.substr(0, SIZE_FORMAT - 1) + ".");
	os << " | ";
	os << std::left << "term: " << std::setw(SIZE_FORMAT) << token.getTerminal()
	   << " | ";
	os << std::left << "line: " << std::setw(SIZE_FORMAT) << token.getLine()
	   << " | ";
	os << std::left << "col: " << std::setw(SIZE_FORMAT) << token.getCol()
	   << " | ";
	os << std::left << "type: " << std::setw(SIZE_FORMAT) << token.getType();

	return (os);
}