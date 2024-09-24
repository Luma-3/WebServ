/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:21:26 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/24 13:22:01 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <exception>
#include <string>

class InternalServerException : public std::exception
{
  private:
	std::string _msg;

  public:
	InternalServerException();
	InternalServerException(const std::string &msg);
	InternalServerException(const InternalServerException &src);
	InternalServerException &operator=(const InternalServerException &src);
	virtual ~InternalServerException() throw();

	virtual const char *what() const throw();
	// TODO : add logger
};

#endif // SERVEREXCEPTION_HPP