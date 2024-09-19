/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requestutils.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:49:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/19 12:53:28 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

template < typename T > std::string ToString(const T &value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

