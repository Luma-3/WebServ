/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:03:20 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 16:27:09 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <iostream>
#include <ostream>
#include <vector>

#include "lexer/Token.hpp"
#include "parser/statement/ErrorPage.hpp"

namespace statement {

class Location : public Token
{
  private:
	const std::string _route;
	std::string		  _root;
	std::string		  _index;
	bool			  _autoindex;
	std::string		  _return;

	std::vector< std::string > _deny_methods;

	std::vector< statement::ErrorPage > _error_pages;

  public:
	Location();
	Location(const Location &src);
	Location(const std::vector< Token * > &tokens, const std::string &route);
	Location &operator=(const Location &src);
	~Location();

	const std::string &getRoute() const
	{
		return _route;
	}

	const std::string &getRoot() const
	{
		return _root;
	}

	const std::string &getIndex() const
	{
		return _index;
	}

	bool getAutoindex() const
	{
		return _autoindex;
	}

	const std::string &getReturn() const
	{
		return _return;
	}

	const std::vector< std::string > &getDenyMethods() const
	{
		return _deny_methods;
	}

	const std::vector< statement::ErrorPage > &getErrorPages() const
	{
		return _error_pages;
	}

  private:
	void IdentifyParam(Token *token);
};

} // namespace statement

std::ostream &operator<<(std::ostream &o, const statement::Location &location);

#endif // LOCATIONCONFIG_HPP