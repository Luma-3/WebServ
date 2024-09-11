/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:49:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/11 13:21:32 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Location.hpp"

Locations::Locations() : _autoindex(false) {}

Locations::~Locations() {}

Locations::Locations(const Locations &src)
{
	_autoindex = src._autoindex;
	_route = src._route;
	_index = src._index;
	// etc...
}

Locations &Locations::operator=(const Locations &src)
{
	if (this != &src) {
		_autoindex = src._autoindex;
		_route = src._route;
		_index = src._index;
		// etc...
	}
	return *this;
}
