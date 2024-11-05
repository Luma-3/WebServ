/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifCGIResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:26:37 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/05 14:28:48 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

namespace {
void buildHeader(std::string &cgiHeader)
{
	std::string http = "HTTP/1.1";

	if (cgiHeader.find("Content-type:") == std::string::npos &&
		cgiHeader.find("Content-Type:") == std::string::npos) {
		throw InternalServerException("CGI: ", __LINE__, __FILE__,
									  "No Content-Type found");
	}
	size_t pos = cgiHeader.find("Status:");
	if (pos != std::string::npos) {
		cgiHeader.erase(pos, 7);
		cgiHeader.insert(pos, http);
	}
	else if (cgiHeader.find("HTTP/1.1") == std::string::npos) {
		cgiHeader.insert(0, "HTTP/1.1 200 OK\r\n");
	}
	if (cgiHeader.find("Content-Length:") == std::string::npos &&
		cgiHeader.find("Transfer-Encoding:") == std::string::npos) {
		cgiHeader += "Transfer-Encoding: chunked\r\n";
	}
	cgiHeader += "\r\n";
}

} // namespace

void CGIHandler::adjustHeader(std::string &client_response)
{
	size_t		pos = _response.find("\r\n\r\n");
	std::string header;
	std::string body;

	if (pos != std::string::npos) {
		header = _response.substr(0, pos + 2);
		std::cerr << "Header: " << header << std::endl;
		body = _response.substr(pos + 4);
	}
	else {
		throw InternalServerException("CGI: ", __LINE__, __FILE__,
									  "No header found");
	}

	buildHeader(header);
	std::cerr << "Header Post: " << header << std::endl;
	client_response = header + body;

	std::cerr << "CGI Response: " << client_response << std::endl;
}