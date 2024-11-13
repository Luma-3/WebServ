/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifCGIResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:26:37 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/13 18:15:52 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

namespace {
void buildHeader(std::string &cgiHeader)
{
	const std::string http = "HTTP/1.1";
	const std::string status = "Status:";
	if (cgiHeader.find("Content-type:") == std::string::npos &&
		cgiHeader.find("Content-Type:") == std::string::npos) {
		throw InternalServerException("CGI: ", __LINE__, __FILE__,
									  "No Content-Type found");
	}
	const size_t pos = cgiHeader.find(status);
	if (pos != std::string::npos) {
		cgiHeader.erase(pos, status.size());
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
	const size_t pos = _response.find("\r\n\r\n");
	std::string	 header;
	std::string	 body;

	if (pos != std::string::npos) {
		header = _response.substr(0, pos + 2);
		body = _response.substr(pos + 4);
	}
	else {
		throw InternalServerException("CGI: ", __LINE__, __FILE__,
									  "No header found");
	}

	buildHeader(header);
	const std::string header_status = header.substr(STATUS_POS, 3);
	if (body.empty() && header_status == "500") {
		throw InternalServerException("CGI: ", __LINE__, __FILE__,
									  "No body found with status: " +
										  header_status);
	}
	client_response = header + body;
}