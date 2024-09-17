/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:38:58 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 11:23:08 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

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
	ErrorPage &operator=(const ErrorPage &src);
	~ErrorPage();
};

} // namespace statement

#endif // ERRORPAGE_HPP