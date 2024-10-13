/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 22:54:59 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/13 12:39:04 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/VirtualServer.hpp"

VirtualServer::VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer &src) :
	_config(src._config), // TODO: deep copy
	_locations(vector_deep_copy(src._locations))
{
}

VirtualServer &VirtualServer::operator=(const VirtualServer &rhs)
{
	if (this == &rhs) return *this;
	_config = rhs._config;
	return *this;
}

bool VirtualServer::operator==(const VirtualServer &rhs) const
{
	return (_config == rhs._config && _locations == rhs._locations);
}

void VirtualServer::addParam(const std::string &key, Param *param)
{
	_config[key] = param;
}

void VirtualServer::addLocation(Location *location)
{
	_locations.push_back(location);
}

const Param *VirtualServer::getParam(const std::string &key) const
{
	try {
		return _config.at(key);
	} catch (std::out_of_range &e) {
		return NULL;
	}
}

const Location *VirtualServer::getLocation(const std::string &path) const
{
	std::vector< Location * >::const_iterator it = _locations.begin();

	while (it != _locations.end()) {
		if ((*it)->getRoute() == path) {
			return *it;
		}
		++it;
	}
	return NULL;
}

void VirtualServer::print() const
{
	std::map< std::string, Param * >::const_iterator it = _config.begin();

	std::cout << "VirtualServer  Param : " << std::endl;
	while (it != _config.end()) {
		std::cout << it->first << " : ";
		it->second->print();
		++it;
	}

	std::vector< Location * >::const_iterator it2 = _locations.begin();

	std::cout << "VirtualServer  Locations : " << std::endl;
	while (it2 != _locations.end()) {
		(*it2)->print();
		++it2;
	}
}

VirtualServer::~VirtualServer()
{
	std::map< std::string, Param * >::iterator it = _config.begin();
	while (it != _config.end()) {
		delete it->second;
		++it;
	}

	std::vector< Location * >::iterator it2 = _locations.begin();
	while (it2 != _locations.end()) {
		delete *it2;
		++it2;
	}
}