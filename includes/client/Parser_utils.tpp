/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
<<<<<<<< HEAD:includes/client/Parser_utils.tpp
/*   Parser_utils.tpp                                   :+:      :+:    :+:   */
========
/*   TestToken.cpp                                      :+:      :+:    :+:   */
>>>>>>>> 50cd1baa8812f985c5207b7f6c8ad130c8558bff:tests/statement/TestToken.cpp
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<<< HEAD:includes/client/Parser_utils.tpp
/*   Created: 2024/09/19 12:49:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/20 15:54:12 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

template < typename T > std::string ToString(const T &value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template < typename T > std::string getAndErase(T &str, const T &delim)
{
	size_t pos = str.find(delim);
	string ret = str.substr(0, pos);

	if (pos == string::npos) {
		return ret;
	}

	size_t new_start = pos + delim.length();
	str = str.substr(new_start);
	return ret;
}
========
/*   Created: 2024/09/23 13:33:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/23 13:34:11 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "lexer/Token.hpp"

// using lexer::Token;

Te
>>>>>>>> 50cd1baa8812f985c5207b7f6c8ad130c8558bff:tests/statement/TestToken.cpp
