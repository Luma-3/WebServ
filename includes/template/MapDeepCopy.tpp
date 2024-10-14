/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapDeepCopy.tpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:30:49 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/14 13:20:40 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_DEEP_COPY_TPP
#define MAP_DEEP_COPY_TPP

#include <map>

template < typename K, typename V >
std::map< K, V > mapDeepCopy(const std::map< K, V > &src)
{
	std::map< K, V > dst;

	for (typename std::map< K, V >::const_iterator it = src.begin();
		 it != src.end(); ++it) {
		dst[it->first] = *new V(it->second);
	}

	return dst;
}

#endif
