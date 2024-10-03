/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToString.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:10:53 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/03 10:38:52 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOSTRING_TPP
#define TOSTRING_TPP

#include <sstream>

template < typename T > std::string ToString(const T &value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
};

#endif // TOSTRING_TPP
