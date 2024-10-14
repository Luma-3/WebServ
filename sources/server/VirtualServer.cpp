/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 22:54:59 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/14 13:33:00 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/VirtualServer.hpp"

#include "template/MapDeepCopy.tpp"

VirtualServer::VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer &src) :
	_config(mapDeepCopy(src._config)),
	_locations(mapDeepCopy(src._locations))
{
}

VirtualServer &VirtualServer::operator=(const VirtualServer &rhs)
{
	if (this == &rhs) return *this;
	_config = mapDeepCopy(rhs._config);
	_locations = mapDeepCopy(rhs._locations);
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

void VirtualServer::addLocation(const std::string &route, Location *location)
{
	_locations[route] = location;
}

const Param *VirtualServer::getParam(const std::string &key) const
{
	try {
		return _config.at(key);
	} catch (std::out_of_range &e) {
		return NULL;
	}
}

const std::string VirtualServer::getParamValue(const std::string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return "";
	}
	return param->getValue();
}

const std::pair< std::string, std::string >
VirtualServer::getParamPair(const std::string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return std::pair< std::string, std::string >("", "");
	}
	return param->getPair();
}

const std::vector< std::string >
VirtualServer::getParamList(const std::string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return std::vector< std::string >();
	}
	return param->getList();
}

const Location *VirtualServer::getLocation(const std::string &path) const
{
	try {
		return _locations.at(path);
	} catch (std::out_of_range &e) {
		return NULL;
	}
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

	std::map< std::string, Location * >::const_iterator it2 =
		_locations.begin();

	std::cout << "VirtualServer  Locations : " << std::endl;
	while (it2 != _locations.end()) {
		std::cout << it2->first << " : ";
		it2->second->print();
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

	std::map< std::string, Location * >::const_iterator it2 =
		_locations.begin();
	;
	while (it2 != _locations.end()) {
		delete (*it2).second;
		++it2;
	}
}