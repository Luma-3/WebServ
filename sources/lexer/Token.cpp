/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:37:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/13 12:17:59 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/Token.hpp"

#include <iostream>

Token::Token() : _type(TOKEN), _key("") {}

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

Token::MissingParamException::MissingParamException() {}

Token::MissingParamException::MissingParamException(const std::string &param) :
	_msg("Missing Param: " + param)
{
}

Token::MissingParamException::MissingParamException(
	const MissingParamException &src)
{
	*this = src;
}

Token::MissingParamException &
Token::MissingParamException::operator=(const MissingParamException &src)
{
	(void)src;
	return (*this);
}

Token::MissingParamException::~MissingParamException() throw() {}

const char *Token::MissingParamException::what() const throw()
{
	return (_msg.c_str());
}