/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VectorDeepCopy.tpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:32:35 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/08 14:19:53 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_DEEP_COPY_TPP
#define VECTOR_DEEP_COPY_TPP

#include <vector>

template < typename T >
std::vector< T * > vectorDeepCopy(const std::vector< T * > &src)
{
	std::vector< T * > dst;
	for (typename std::vector< T * >::const_iterator it = src.begin();
		 it != src.end(); ++it) {
		dst.push_back(new T(**it));
	}
	return dst;
}

template < typename T >
std::vector< T > vectorDeepCopy(const std::vector< T > &src)
{
	std::vector< T > dst;
	for (typename std::vector< T >::const_iterator it = src.begin();
		 it != src.end(); ++it) {
		dst.push_back(T(*it));
	}
	return dst;
}

template < typename T > void deleteVector(const std::vector< T > &src)
{
	for (typename std::vector< T >::const_iterator it = src.begin();
		 it != src.end(); ++it) {
		delete *it;
	}
}

#endif // VECTOR_DEEP_COPY_TPP
