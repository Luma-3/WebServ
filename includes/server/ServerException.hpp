/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:21:26 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/29 10:26:59 by Monsieur_Ca      ###   ########.fr       */
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
	InternalServerException(const std::string &function, int line,
							const std::string &file, const std::string &error);
	InternalServerException(const InternalServerException &src);
	InternalServerException &operator=(const InternalServerException &src);
	virtual ~InternalServerException() throw();

	virtual const char *what() const throw();
	// TODO : add logger
};

class RecvException : public std::exception
{
  private:
	std::string _msg;

  public:
	RecvException();
	RecvException(const std::string &function, int line,
				  const std::string &file, const std::string &error);
	RecvException(const RecvException &src);
	RecvException &operator=(const RecvException &src);
	virtual ~RecvException() throw();

	virtual const char *what() const throw();
};

#endif // SERVEREXCEPTION_HPP