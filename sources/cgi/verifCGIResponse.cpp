/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifCGIResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:26:37 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/16 16:41:04 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGI.hpp"

bool asContentType(const std::string &header)
{
	return (header->find("Content-Type:") != std::string::npos);
}

void adjustHeader(const std::string *response)
{
	size_t pos = response->find("\r\n\r\n");
	if (pos == std::string::npos) {
		return; // TODO error CGI
	}
	std::string header = response->substr(0, pos);

	if (!asContentType(&header)) {
		// TODO error CGI
	}

	_reponse
}