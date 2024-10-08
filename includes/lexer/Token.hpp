/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 14:37:24 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/08 10:43:45 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <ostream>
#include <string>

enum Token_Type {
	S_Terminal,
	S_Parameter,
	S_ErrorPage,
	S_DenyMethod,
	S_Return,
	S_Location,
	S_Server,
	S_Log
};

enum Terminal_Type {
	T_None,

	T_Server,
	T_Location,
	T_ErrorPage,
	T_DenyMethod,
	T_Return,
	T_Port,
	T_Host,
	T_Index,
	T_AutoIndex,
	T_Root,
	T_Log,
	T_BodySize,
	T_Name,

	T_Comma,
	T_Semi_Colon,
	T_CBracket,
	T_OBracket,
	T_OSquareBracket,
	T_CSquareBracket,

	T_Identifier,
	T_Digits,
	T_Method,
	T_Path,
	T_Bool,
	T_LogLevel
};

struct IdentifyRegex {
	enum Terminal_Type type;
	bool (*func)(const std::string &);
};

struct IdentifyKey {
	enum Terminal_Type type;
	std::string		   key;
};

struct IdentifyTypeTerminal {
	enum Terminal_Type type;
	std::string		   term;
};

#define PORT_MAX 65535

class Token
{
  private:
	std::string		   _value;
	enum Token_Type	   _type;
	enum Terminal_Type _terminal;
	size_t			   _line;
	size_t			   _col;

  public:
	Token();
	Token(const Token &src);
	Token(enum Token_Type type);
	Token(const std::string &value, enum Terminal_Type terminal, size_t line,
		  size_t col);
	Token(enum Token_Type type, enum Terminal_Type terminal);

	Token &operator=(const Token &src);

	virtual const std::string &getValue() const;
	enum Token_Type			   getType() const;
	enum Terminal_Type		   getTerminal() const;
	size_t					   getLine() const;
	size_t					   getCol() const;

	static Terminal_Type IdentifyTerminal(const std::string &value);
	static bool			 IsKey(const Token &token);

	static const std::string &TerminalTypeToString(enum Terminal_Type type);

	class InvalidTokenException : public std::exception
	{
	  public:
		InvalidTokenException();
		InvalidTokenException(const InvalidTokenException &src);
		virtual ~InvalidTokenException() throw();
		InvalidTokenException &operator=(const InvalidTokenException &src);
		virtual const char	  *what() const throw();
	};

	virtual ~Token();
};

std::ostream &operator<<(std::ostream &os, const Token &token);

bool IsBool(const std::string &value);

bool IsMethod(const std::string &value);

bool IsLogLevel(const std::string &value);

bool IsPath(const std::string &value);

bool IsDigit(const std::string &value);

bool IsErrorCode(const std::string &value);

bool IsPort(const std::string &value);

bool IsHost(const std::string &value);

bool IsBodySize(const std::string &value);

#endif // TOKEN_HPP