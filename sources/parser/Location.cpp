/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:10:50 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/19 16:43:23 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Location.hpp"

#include "lexer/Token.hpp"
#include "template/VectorDeepCopy.tpp"

Location::Location(const std::string &route) : _route(route) {}

Location::Location(const Location &src)
{
	*this = src;
}

Location &Location::operator=(const Location &src)
{
	if (this != &src) {
		return *this;
	}
	return *this;
}

bool Location::operator==(const Location &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	for (std::map< std::string, Param * >::const_iterator it = _params.begin();
		 it != _params.end(); ++it) {
		if (it->second != rhs._params.at(it->first)) {
			return false;
		}
	}
	return true;
}

void Location::addParam(const std::string &key, Param *param)
{
	_params[key] = param;
}

const Param *Location::getParam(const std::string &key) const
{
	try {
		return _params.at(key);
	} catch (std::out_of_range &e) {
		return NULL;
	}
}

const std::string Location::getParamValue(const std::string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return "";
	}
	return param->getValue();
}

const std::pair< std::string, std::string >
Location::getParamPair(const std::string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return std::pair< std::string, std::string >("", "");
	}
	return param->getPair();
}

const std::vector< std::string >
Location::getParamList(const std::string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return std::vector< std::string >();
	}
	return param->getList();
}

std::string Location::getRoot(const std::string &path) const
{
	std::string root;
	std::string final;

	try {
		root = _params.at("root")->getValue();
	} catch (std::out_of_range &e) {
		return "";
	}
	final = path;
	size_t patern_pos = final.find_first_of(_route);
	std::cout << "Patern: " << _route << std::endl;
	final.replace(patern_pos, _route.size(), root);
	std::cout << "Root: " << final << std::endl;
	return (final);
}

void Location::print() const
{
	std::map< std::string, Param * >::const_iterator it = _params.begin();

	while (it != _params.end()) {
		std::cout << "Key : " << it->first << " Value : ";
		it->second->print();
		++it;
	}
}

Location::~Location()
{
	for (std::map< std::string, Param * >::iterator it = _params.begin();
		 it != _params.end(); ++it) {
		delete it->second;
	}
}
