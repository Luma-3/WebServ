/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:09:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/24 15:08:27 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/ServerException.hpp"

InternalServerException::InternalServerException() :
	_msg("Internal Server Error")
{
}

InternalServerException::InternalServerException(const std::string &msg) :
	_msg(msg)
{
}

InternalServerException::InternalServerException(
	const InternalServerException &src) :
	_msg(src._msg)
{
}

InternalServerException &
InternalServerException::operator=(const InternalServerException &src)
{
	if (this == &src) {
		return (*this);
	}
	_msg = src._msg;
	return (*this);
}

InternalServerException::~InternalServerException() throw() {}

const char *InternalServerException::what() const throw()
{
	return _msg.c_str();
}
