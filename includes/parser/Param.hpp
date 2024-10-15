/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 22:32:19 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/15 13:52:27 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAM_HPP
#define PARAM_HPP

#include <string>
#include <vector>

#include "parser/IParserToken.hpp"

class Param : public IParserToken
{
  private:
	enum TokenType						  _type;
	std::string							  _key;
	std::string							  _value;
	std::pair< std::string, std::string > _pair;
	std::vector< std::string >			  _list;

  public:
	Param(const std::string &key, const std::string &value);
	Param(const std::string &key, const std::string &value1,
		  const std::string &value2);
	Param(const std::string &key, const std::vector< std::string > &list);
	Param(const Param &src);
	Param &operator=(const Param &rhs);
	~Param();

	bool operator==(const Param &rhs) const;

	TokenType		   getType() const { return _type; };
	const std::string &getKey() const { return _key; };
	const std::string &getValue() const { return _value; };
	const std::pair< std::string, std::string > &getPair() const
	{
		return _pair;
	};
	const std::vector< std::string > &getList() const { return _list; };
	Terminal_Type					  getTerminal() const { return T_None; };

	void print() const;
};

#endif // PARAM_HPP