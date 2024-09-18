/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 14:37:24 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 17:00:07 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

enum Token_Type {
	S_Terminal,
	S_Parameter,
	S_ErrorPage,
	S_DenyMethod,
	S_Return,
	S_Location,
	S_Server,
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
	T_ErroLog,
	T_AccessLog,

	T_Comma,
	T_Semi_Colon,
	T_CBracket,
	T_OBracket,
	T_Colone,
	T_Equal,

	T_Value,
	T_ErrorCode,
	T_Method,
	T_FileName,
	T_PortValue,
	T_Path,
	T_FilePath,
	T_HostValue,
	T_Bool,
};

struct IdentifyRegex {
	enum Terminal_Type type;
	bool (*func)(const std::string &);
};

struct IdentifyKey {
	enum Terminal_Type type;
	const std::string  key;
};

#define PORT_MAX 65535

class Token
{
  private:
	std::string _value;

  protected:
	enum Token_Type	   _type;
	enum Terminal_Type _terminal;

  public:
	Token();
	Token(const Token &src);
	Token(enum Token_Type type);
	Token(const std::string &value, enum Terminal_Type terminal);
	Token(enum Token_Type type, enum Terminal_Type terminal);

	Token &operator=(const Token &src);

	enum Token_Type			   getType() const;
	enum Terminal_Type		   getTerminal() const;
	virtual const std::string &getValue() const;

	static Terminal_Type IdentifyTerminal(const std::string &value);

	static bool IsKey(const Token &token);

	virtual ~Token();
};

bool IsPort(const std::string &value);

bool IsHost(const std::string &value);

bool IsErrorCode(const std::string &value);

bool IsFilePath(const std::string &value);

bool IsPath(const std::string &value);

bool IsBool(const std::string &value);

bool IsMethod(const std::string &value);

bool IsFileName(const std::string &value);

#endif // TOKEN_HPP