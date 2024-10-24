/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:37:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/24 13:42:58 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Token.hpp"

#include <iostream>

#include "color.hpp"
#include "template/StringUtils.tpp"

Token::Token() : _type(TOKEN), _key("") {}

Token::Token(const Terminal_Type term) : _type(TOKEN), _key(""), _terminal(term)
{
}
Token::Token(const std::string &key, Terminal_Type term, int line, int col) :
	_type(TOKEN),
	_key(key),
	_terminal(term),
	_line(line),
	_col(col)
{
}

Token::Token(const Token &src) : _type(src._type), _key(src._key) {}

Token &Token::operator=(const Token &rhs)
{
	if (this == &rhs) return *this;
	_type = rhs._type;
	_key = rhs._key;
	return *this;
}

Token::~Token() {}

bool Token::IsKey(const IParserToken &token)
{
	const Terminal_Type type = token.getTerminal();

	if (type <= T_BodySize && type >= T_Server) {
		return (true);
	}
	return (false);
}

void Token::print() const
{
	std::cout << "Token value: " << _key << std::endl;
}

std::string Token::TerminalToString(Terminal_Type terminal)
{
	static const int				  size = 25;
	static const IdentifyTypeTerminal key[size] = {
		{T_Server,		   "server"						   },
		{T_Location,		 "location"					   },
		{T_ErrorPage,	  "error_page"					  },
		{T_DenyMethod,	   "deny_method"					},
		{T_Return,		   "return"						   },
		{T_Listen,		   "listen"						   },
		{T_Hostname,		 "hostname"					   },
		{T_Index,		  "index"						 },
		{T_AutoIndex,	  "autoindex"					 },
		{T_Root,			 "root"						  },
		{T_Log,			"log"						   },
		{T_BodySize,		 "max_body_size"					},
		{T_LogLevel,		 "DEBUG | INFO | WARNING | ERROR"},
		{T_Method,		   "POST | GET | DELETE"			},
		{T_Path,			 "{path}"						 },
		{T_Bool,			 "on | off"					   },
		{T_Digits,		   "{digits}"						 },
		{T_Identifier,	   "{identifier}"					 },
		{T_Comma,		  ","							 },
		{T_Semi_Colon,	   ";"							 },
		{T_Colon,		  ":"							 },
		{T_OBracket,		 "{"							 },
		{T_CBracket,		 "}"							 },
		{T_OSquareBracket, "["							  },
		{T_CSquareBracket, "]"							  }
	   };

	for (size_t i = 0; i < size; ++i) {
		if (terminal == key[i].type) {
			return (key[i].term);
		}
	}
	return ("");
}

Terminal_Type Token::IdentifyTerminal(const std::string &value)
{
	static const int		 size_key = 12;
	static const IdentifyKey key[size_key] = {
		{T_Server,	   "server"	   },
		{T_Location,	 "location"	   },
		{T_ErrorPage,  "error_page"	  },
		{T_DenyMethod, "deny_method"	},
		{T_Return,	   "return"	   },
		{T_Listen,	   "listen"	   },
		{T_Hostname,	 "hostname"	   },
		{T_Index,	  "index"		 },
		{T_AutoIndex,  "autoindex"	 },
		{T_Root,		 "root"		   },
		{T_Log,		"log"			 },
		{T_BodySize,	 "max_body_size"}
	 };

	for (size_t i = 0; i < size_key; ++i) {
		if (value == key[i].key) {
			return (key[i].type);
		}
	}

	static const int		   size_regexs = 5;
	static const IdentifyRegex regexs[size_regexs] = {
		{T_Bool,	 IsBool	   },
		  {T_Method,	 IsMethod	 },
		{T_LogLevel, IsLogLevel},
		{T_Path,	 IsPath	   },
		  {T_Digits,	 IsDigit	},
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
		case ';':
			return (T_Semi_Colon);
		case ':':
			return (T_Colon);
		case '{':
			return (T_OBracket);
		case '}':
			return (T_CBracket);
		case '[':
			return (T_OSquareBracket);
		case ']':
			return (T_CSquareBracket);
		default:
			break;
	}
	return (T_Identifier);
}

// Exceptions

Token::InvalidTokenException::InvalidTokenException() {}

Token::InvalidTokenException::InvalidTokenException(const std::string &error,
													const std::string &expected,
													const std::string &value,
													int col, int line)
{
	_msg = PASTEL_RED "Error: " ORANGE "Invalid Token: " RESET + error +
		   "\nline: " + ToString(line) + " | col: " + ToString(col) + " >>> " +
		   value + expected + RESET;
}

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
	return (_msg.c_str());
}
