/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:38:58 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/21 13:39:37 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "lexer/Token.hpp"

namespace statement {

class ErrorPage : public Token
{
  private:
	std::vector< std::string > _error_code;
	std::string				   _value;

  public:
	ErrorPage();
	ErrorPage(const ErrorPage &src);
	ErrorPage(const std::vector< std::string > &error_code,
			  const std::string				   &value);
	ErrorPage &operator=(const ErrorPage &src);

	const std::vector< std::string > &getErrorCode() const
	{
		return _error_code;
	}

	const std::string &getValue() const
	{
		return _value;
	}

	~ErrorPage();
};

} // namespace statement

std::ostream &operator<<(std::ostream				&o,
						 const statement::ErrorPage &error_page);

#endif // ERRORPAGE_HPP