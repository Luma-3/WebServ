/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:03:20 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 12:18:46 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

namespace statement {

#include <vector>

#include "lexer/Token.hpp"
#include "parser/statement/ErrorPage.hpp"

class Location : public Token
{
  private:
	std::string _route;
	std::string _root;
	std::string _index;
	std::string _autoindex;
	std::string _return;

	std::vector< std::string > _deny_methods;

	std::vector< statement::ErrorPage > _error_pages;

  public:
	Location();
	Location(const Location &src);
	Location &operator=(const Location &src);
	~Location();
};

} // namespace statement

#endif // LOCATIONCONFIG_HPP