/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IError.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:58:41 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/24 10:57:48 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IERROR_HPP
#define IERROR_HPP

#include <exception>
#include <string>

class IError : public std::exception
{
  private:
	virtual const void logging() const = 0;

  public:
	virtual const char *what() const throw() = 0;
	virtual ~IError() throw() {}
};

#endif