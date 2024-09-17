/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 14:37:24 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 14:45:13 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum Token_Type {
	T_None,
	// Terminal
	T_Key,
	T_Value,
	T_Comma,
	T_Semi_Colon,
	T_Bracket,
	T_Colone,
	T_Equal,
	T_ErrorCode,
	T_Method,
	T_FileName,
	T_Port,
	// Non Terminal
	T_Parameter,
	T_ErrorPage,
	T_DenyMethod,
	T_Location,
	T_ServerBlock,
};

class Token
{
  private:
	std::string _value;

  protected:
	enum Token_Type _type;

  public:
	Token();
	Token(const Token &src);
	Token(enum Token_Type type);
	Token(const std::string &value, enum Token_Type type);

	Token &operator=(const Token &src);

	enum Token_Type	   getType() const;
	const std::string &getValue() const;

	static Token_Type IdentifyToken(const string &value);

	virtual ~Token();
};

#endif // TOKEN_HPP