/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:03:20 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/18 12:21:55 by jbrousse         ###   ########.fr       */
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
	std::string						 _route;
	std::map< std::string, Param * > _params;

  public:
	Location(const std::string &route);
	~Location();

	bool operator==(const Location &rhs) const;

	void addParam(const std::string &key, Param *param);

	std::pair< std::string, std::string >
							   getParamPair(const std::string &key) const;
	std::vector< std::string > getParamList(const std::string &key) const;
	const Param				  *getParam(const std::string &key) const;
	std::string				   getParamValue(const std::string &key) const;
	std::string				   getRoot(const std::string &path) const;

	void print() const;
};

#include "template/Cast.tpp"

#endif // LOCATIONCONFIG_HPP