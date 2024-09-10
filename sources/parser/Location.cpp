/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:49:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 11:50:55 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Locations::Locations() :
	_route(""),
	_root(""),
	_index(""),
	_autoindex(false),
	_methods(),
	_error_pages(),
	_return()
{
}