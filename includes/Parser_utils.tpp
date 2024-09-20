/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_utils.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
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