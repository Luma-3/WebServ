/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:07:07 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/19 13:02:43 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FINDER_HPP
#define FINDER_HPP

#include <string>

#include "server/VirtualServer.hpp"

std::string findParam(const std::string &param, const std::string &uri,
					  const VirtualServer *server);

std::string findRoot(const std::string &uri, const VirtualServer *server);

std::vector< std::string > findParamList(const std::string	 &param,
										 const std::string	 &uri,
										 const VirtualServer *server);

#endif // FINDER_HPP