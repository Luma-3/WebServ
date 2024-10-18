/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IParserToken.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 18:53:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/13 11:35:53 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPARAM_HPP
#define IPARAM_HPP

#include <string>

enum TokenType {
	SIMPLE,
	PAIR,
	LIST,
	TOKEN
};

enum Terminal_Type {
	T_None,

	T_Server,
	T_Location,
	T_ErrorPage,
	T_DenyMethod,
	T_Return,
	T_Listen,
	T_Hostname,
	T_Index,
	T_AutoIndex,
	T_Root,
	T_Log,
	T_BodySize,

	T_Comma,
	T_Colon,
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

class IParserToken
{
  public:
	virtual TokenType		   getType() const = 0;
	virtual const std::string &getKey() const = 0;
	virtual void			   print() const = 0;
	virtual Terminal_Type	   getTerminal() const = 0;
	virtual ~IParserToken() {}
};

#endif // IPARAM_HPP