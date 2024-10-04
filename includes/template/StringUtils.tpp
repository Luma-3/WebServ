/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.tpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:10:53 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/04 12:38:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGUTILS_TPP
#define STRINGUTILS_TPP

#include <sstream>

template < typename T > std::string ToString(const T &value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
};

template < typename T > std::string getAndErase(T &str, const T &delim)
{
	size_t		pos = str.find(delim);
	std::string ret = str.substr(0, pos);

	if (pos == std::string::npos) {
		return ret;
	}

	size_t new_start = pos + delim.length();
	str = str.substr(new_start);
	return ret;
}

#endif // STRINGUTILS_TPP
