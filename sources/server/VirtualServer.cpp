/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 22:54:59 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/18 15:12:36 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/VirtualServer.hpp"

#include "Logger.hpp"

VirtualServer::VirtualServer() {}

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

std::string VirtualServer::getParamValue(const std::string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return "";
	}
	return param->getValue();
}

std::pair< std::string, std::string >
VirtualServer::getParamPair(const std::string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return std::pair< std::string, std::string >("", "");
	}
	return param->getPair();
}

std::vector< std::string >
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
	std::string root;
	std::string final;

	try {
		root = _config.at("root")->getValue();
	} catch (std::out_of_range &e) {
		return "";
	}
	if (path[0] != '/') {
		final = root + path;
	}
	else {
		final = root + path.substr(1);
	}
	return (final);
}

const Location *VirtualServer::getLocation(const std::string &path) const
{
	std::string to_test = path;
	size_t		pos = 0;

	while (!to_test.empty()) {

		try {
			return _locations.at(to_test);
		} catch (std::out_of_range &e) {
			LOG_DEBUG("Location not found: " + to_test);
		}

		pos = to_test.find_last_of('/', to_test.length() - 2);
		to_test = to_test.substr(0, pos + 1);
		if (to_test == "/") {
			break;
		}
	}
	return (NULL);
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

	while (it2 != _locations.end()) {
		delete (*it2).second;
		++it2;
	}
}