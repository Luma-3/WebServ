/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:00:44 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/13 10:23:49 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Param.hpp"

#include <iostream>

Param::Param(const std::string &key, const std::string &value) :
	_type(SIMPLE),
	_key(key),
	_value(value)
{
}

Param::Param(const std::string &key, const std::string &value1,
			 const std::string &value2) :
	_type(PAIR),
	_key(key)
{
	_pair.first = value1;
	_pair.second = value2;
}

Param::Param(const std::string &key, const std::vector< std::string > &list) :
	_type(LIST),
	_key(key),
	_list(list)
{
}

Param::Param(const Param &src) :
	_type(src._type),
	_key(src._key),
	_value(src._value)
{
}

Param &Param::operator=(const Param &rhs)
{
	if (this == &rhs) return *this;
	_type = rhs._type;
	_key = rhs._key;
	_value = rhs._value;
	return *this;
}

Param::~Param() {}

bool Param::operator==(const Param &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	if (_type != rhs._type) {
		return false;
	}
	if (_key != rhs._key) {
		return false;
	}
	if (_value != rhs._value) {
		return false;
	}
	if (_pair.first != rhs._pair.first) {
		return false;
	}
	if (_pair.second != rhs._pair.second) {
		return false;
	}
	if (_list != rhs._list) {
		return false;
	}
	return true;
}

void Param::print() const
{
	if (_type == SIMPLE)
		std::cout << "Simple Param: " << _key << " = " << _value << std::endl;
	else if (_type == PAIR)
		std::cout << "Pair Param: " << _key << " = " << _pair.first << " "
				  << _pair.second << std::endl;
	else if (_type == LIST) {
		std::cout << "List Param: " << _key << " = ";
		for (std::vector< std::string >::const_iterator it = _list.begin();
			 it != _list.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
}
