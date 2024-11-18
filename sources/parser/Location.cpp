/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:10:50 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/18 12:24:37 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Location.hpp"

#include "lexer/Token.hpp"

using std::map;
using std::pair;
using std::string;
using std::vector;

Location::Location(const string &route) : _route(route) {}

bool Location::operator==(const Location &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	for (map< string, Param * >::const_iterator it = _params.begin();
		 it != _params.end(); ++it) {
		if (it->second != rhs._params.at(it->first)) {
			return false;
		}
	}
	return true;
}

void Location::addParam(const string &key, Param *param)
{
	_params[key] = param;
}

const Param *Location::getParam(const string &key) const
{
	try {
		return _params.at(key);
	} catch (std::out_of_range &e) {
		return NULL;
	}
}

string Location::getParamValue(const string &key) const
{
	const Param *param = getParam(key);
	if (param == NULL) {
		return "";
	}
	return param->getValue();
}

pair< string, string > Location::getParamPair(const string &key) const
{
	const Param *param = getParam(key);
	return ((param == NULL) ? pair< string, string >("", "")
							: param->getPair());
}

vector< string > Location::getParamList(const string &key) const
{
	const Param *param = getParam(key);
	return ((param == NULL) ? vector< string >() : param->getList());
}

string Location::getRoot(const string &path) const
{
	string root;
	string final;

	try {
		root = _params.at("root")->getValue();
	} catch (std::out_of_range &e) {
		return "";
	}
	final = path;
	const size_t patern_pos = final.find_first_of(_route);
	final.replace(patern_pos, _route.size(), root);
	return (final);
}

void Location::print() const
{
	map< string, Param * >::const_iterator it = _params.begin();

	while (it != _params.end()) {
		std::cout << "Key : " << it->first << " Value : ";
		it->second->print();
		++it;
	}
}

Location::~Location()
{
	for (map< string, Param * >::iterator it = _params.begin();
		 it != _params.end(); ++it) {
		delete it->second;
	}
}
