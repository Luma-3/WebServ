/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:07:07 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/20 17:57:29 by anthony          ###   ########.fr       */
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

std::string findErrorParam(const std::string   &code,
						   const std::string   &requestPath,
						   const VirtualServer *server);

#endif // FINDER_HPP