/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.tpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:10:53 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/13 10:12:41 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGUTILS_TPP
#define STRINGUTILS_TPP

#include <sstream>
#include <string>

template < typename T > std::string ToString(const T &value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template < typename T > std::string getAndErase(T &str, const T &delim)
{
	const size_t	  pos = str.find(delim);
	const std::string ret = str.substr(0, pos);

	if (pos == std::string::npos) {
		return ret;
	}

	const size_t new_start = pos + delim.length();
	str = str.substr(new_start);
	return ret;
}

#endif // STRINGUTILS_TPP
