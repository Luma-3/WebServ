/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 14:37:24 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/16 15:08:47 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum Token_Type {
	None,
	Key,
	Value,
	Comma,
	Semi_Colon,
	Bracket,
	Colone,
	Equal
};

class Token
{
  private:
	enum Token_Type _type;
	std::string		_value;

  public:
	Token();
	Token(const Token &src);
	Token(const std::string &value, enum Token_Type type);

	enum Token_Type	   getType() const;
	const std::string &getValue() const;

	Token &operator=(const Token &src);
	virtual ~Token();
};

#endif // TOKEN_HPP