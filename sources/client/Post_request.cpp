/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:10:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/29 16:39:13 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>

#include "client/Client.hpp"

using client::Client;
using std::string;

int Client::handlePostRequest(const client::Parser &parser)
{
	int max_body_size = 0;
	int content_size;

	string content_length = parser.getHeader("Content-Length");
	if (content_length.empty()) {
		return 411;
	}

	content_size = atoi(content_length.c_str());
	const Location *location = _server->getLocation(parser.getRequestedPath());
	if (location != NULL) {
		max_body_size = atoi(location->getParamValue("max_body_size").c_str());
	}
	else if (!_server->getParamValue("max_body_size").empty()) {
		max_body_size = atoi(_server->getParamValue("max_body_size").c_str());
	}

	if (max_body_size < content_size) {
		return 413;
	}

	std::cout
		<< "Je suis dans le post et je creer un nouveau cgi handler putain"
		<< std::endl;
	_cgi_handler = new CGIHandler(this, const_cast< client::Parser * >(&parser),
								  _server, _builder);
	if (_cgi_handler->execute() == FAILURE) {
		std::cout << "VERIFFFFFFFFFFFFFFFFFFFFFFFFFF" << std::endl;
		return 500;
	}
	std::cout << "Je suis a la fiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiin post et je "
				 "creer un nouveau cgi handler putain"
			  << std::endl;
	return SUCCESS;
}