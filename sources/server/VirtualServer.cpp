/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 22:54:59 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/18 12:44:13 by jbrousse         ###   ########.fr       */
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
	location->setRoute(route);
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

std::string VirtualServer::getRoot(const std::string &path) const
{
	std::string to_test = path;
	size_t		pos;

	while (to_test.empty() != true) {
		try {
			return _locations.at(to_test);
		} catch (...) {
		}
		pos = to_test.find_last_of("/", to_test.length() - 2);
		to_test = to_test.substr(0, pos + 1);
	}
	return (NULL);
}

std::string VirtualServer::getRoot(const std::string &path) const
{
	std::string root;
	std::string final;

	try {
		root = _config.at("root")->getValue();
	} catch (std::out_of_range &e) {
		return "";
	}
	final = root + path;
	return (final);
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