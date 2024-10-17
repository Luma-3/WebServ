/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:03:20 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/17 16:37:37 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <iostream>
#include <map>
#include <ostream>
#include <vector>

#include "lexer/Token.hpp"
#include "parser/IConfig.hpp"
#include "parser/Param.hpp"
class Location : public IConfig
{
  private:
	std::map< std::string, Param * > _params;
	std::string						 _route;

  public:
	// void IdentifyParam(Token *token);

  public:
	Location();
	Location(const Location &src);
	Location &operator=(const Location &src);
	~Location();

	bool operator==(const Location &rhs) const;

	void			  addParam(const std::string &key, Param *param);
	const Param		 *getParam(const std::string &key) const;
	const std::string getParamValue(const std::string &key) const;
	const std::pair< std::string, std::string >
									 getParamPair(const std::string &key) const;
	const std::vector< std::string > getParamList(const std::string &key) const;
	std::string						 getRoot(const std::string &path) const;
	void							 print() const;
	void setRoute(const std::string &route) { _route = route; };
};

#include "template/Cast.tpp"

#endif // LOCATIONCONFIG_HPP