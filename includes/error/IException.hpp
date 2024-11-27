/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IException.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:58:41 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/27 11:07:06 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEXCEPTION_HPP
#define IEXCEPTION_HPP

#include <exception>
#include <string>

class IException : public std::exception
{
  private:
	virtual const void logging() const = 0;

  public:
	virtual const char *what() const throw() = 0;
	virtual ~IException() throw() {}
};

#endif
