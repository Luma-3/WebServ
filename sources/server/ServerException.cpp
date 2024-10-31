/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:09:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/31 11:04:43 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/ServerException.hpp"

#include "color.hpp"
#include "template/StringUtils.tpp"

InternalServerException::InternalServerException() :
	_msg("Internal Server Error")

{
}

InternalServerException::InternalServerException(const std::string &function,
												 int				line,
												 const std::string &file,
												 const std::string &error)
{
	std::string new_file(file);
	size_t		pos = new_file.find_last_of("/");
	if (pos != std::string::npos) {
		new_file = new_file.substr(pos + 1);
	}

	_msg = PASTEL_RED "Error: " ORANGE "Internal Server Exception\n" RESET +
		   function + " at " + new_file + " | Line: " + ToString(line) + ": " +
		   error;
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



RecvException::RecvException() : _msg("Recv Error") {}

RecvException::RecvException(const std::string &function, int line,
							 const std::string &file, const std::string &error)
{
	std::string new_file(file);
	size_t		pos = new_file.find_last_of("/");
	if (pos != std::string::npos) {
		new_file = new_file.substr(pos + 1);
	}

	_msg = PASTEL_RED "Error: " ORANGE "Recv Exception\n" RESET + function +
		   " at " + new_file + " | Line: " + ToString(line) + ": " + error;

}

RecvException::RecvException(const RecvException &src) :
	_msg(src._msg)
{
}

RecvException &RecvException::operator=(const RecvException &src)
{
	if (this == &src) {
		return (*this);
	}
	_msg = src._msg;
	return (*this);
}

RecvException::~RecvException() throw() {}

const char *RecvException::what() const throw()
{
	return _msg.c_str();
}

