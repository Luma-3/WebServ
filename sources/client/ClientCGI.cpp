/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:43:59 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/28 09:45:23 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

using client::Client;

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

bool logicalEmptyBody(const std::string &status)
{
	static const std::string errorCodes[] = {"100", "101", "102", "200", "204",
											 "205", "208", "300", "301", "302",
											 "304", "307", "308", "310"};
	const size_t			 size = sizeof(errorCodes) / sizeof(errorCodes[0]);

	for (size_t i = 0; i < size; i++) {
		if (status == errorCodes[i]) {
			return true;
		}
	}
	return false;
}

} // namespace

void Client::adjustCGIResponse()
{
	int	   size_delimiter = 4;
	size_t pos = _response.find("\r\n\r\n");

	if (pos == std::string::npos) {
		pos = _response.find("\n\n");
		size_delimiter = 2;
	}
	std::string header;
	std::string body;

	if (pos != std::string::npos) {
		header = _response.substr(0, pos + (size_delimiter / 2));
		body = _response.substr(pos + size_delimiter);
	}
	else {
		_builder->setCode("502");
		_builder->findErrorPage();
		_builder->BuildResponse(_response);
		return;
	}

	try {
		buildHeader(header);
	} catch (const std::exception &e) {
		_builder->setCode("502");
		_builder->findErrorPage();
		_builder->BuildResponse(_response);
		return;
	}

	const std::string status = header.substr(STATUS_POS, 3);
	if (body.empty() && !logicalEmptyBody(status)) {
		_builder->setCode(status);
		_builder->findErrorPage();
		_builder->BuildResponse(_response);
		return;
	}
	_response = header + body;
}
