/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_utils.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:41:21 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/24 15:41:33 by jbrousse         ###   ########.fr       */
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