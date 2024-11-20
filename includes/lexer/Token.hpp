/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:34:22 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/20 13:37:12 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

#include "parser/IParserToken.hpp"

struct IdentifyRegex {
	enum Terminal_Type type;
	bool (*func)(const std::string &);
};

// NOLINTSTART
struct IdentifyKey {
	enum Terminal_Type type;
	std::string		   key;
};
// NOLINTEND

struct IdentifyTypeTerminal {
	enum Terminal_Type type;
	std::string		   term;
};

class Token : public IParserToken
{
  private:
	enum TokenType	   _type;
	std::string		   _key;
	enum Terminal_Type _terminal;

	size_t _line;
	size_t _col;

  public:
	Token();
	Token(const std::string &key, Terminal_Type term, size_t line, size_t col);
	Token(Terminal_Type term);
	Token(const Token &src);
	Token &operator=(const Token &rhs);
	~Token();

	TokenType		   getType() const { return _type; };
	const std::string &getKey() const { return _key; };
	Terminal_Type	   getTerminal() const { return _terminal; };
	size_t			   getLine() const { return _line; };
	size_t			   getCol() const { return _col; };
	static std::string TerminalToString(Terminal_Type terminal);

	static bool			 IsKey(const IParserToken &token);
	static Terminal_Type IdentifyTerminal(const std::string &value);

	void print() const;

	class InvalidTokenException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		InvalidTokenException();
		InvalidTokenException(const std::string &error,
							  const std::string &expected,
							  const std::string &value, size_t col,
							  size_t line);
		InvalidTokenException(const InvalidTokenException &src);
		virtual ~InvalidTokenException() throw();
		InvalidTokenException &operator=(const InvalidTokenException &src);
		virtual const char	  *what() const throw();
	};

	class MissingParamException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		MissingParamException();
		MissingParamException(const std::string &param);
		MissingParamException(const MissingParamException &src);
		virtual ~MissingParamException() throw();
		MissingParamException &operator=(const MissingParamException &src);
		virtual const char	  *what() const throw();
	};
};

bool IsBool(const std::string &value);

bool IsMethod(const std::string &value);

bool IsLogLevel(const std::string &value);

bool IsPath(const std::string &value);

bool IsDigit(const std::string &value);

bool IsErrorCode(const std::string &value);

bool IsHostname(const std::string &value);

bool IsBodySize(const std::string &value);

bool IsIP(const std::string &value);

bool IsPort(const std::string &value);

bool IsCGIExtension(const std::string &value);

#endif // TOKEN_HPP