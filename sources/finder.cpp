/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:08:55 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/18 10:18:34 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "finder.hpp"

std::string findParam(const std::string &param, const std::string &uri,
					  const VirtualServer *server)
{
	const Location *location = server->getLocation(uri);
	std::string		value;

	if (location != NULL) {
		value = location->getParamValue(param);
		if (!value.empty()) {
			return value;
		}
	}
	return server->getParamValue(param);
}

std::string findRoot(const std::string &uri, const VirtualServer *server)
{
	const Location *location = server->getLocation(uri);
	std::string		root;

	if (location != NULL) {
		root = location->getRoot(uri);
		if (!root.empty()) {
			return root;
		}
	}
	return server->getRoot(uri);
}