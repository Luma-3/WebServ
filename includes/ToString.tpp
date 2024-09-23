/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToString.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:10:53 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/20 16:16:29 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

template < typename T > std::string ToString(const T &value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
};