/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifCGIResponse.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:26:37 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/17 16:46:46 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGI.hpp"

std::string findHeaderElement(const std::string &header, std::string element)
{
	size_t		pos = header.find(element);
	std::string found;

	if (pos == std::string::npos) {
		return "";
	}
	pos += element.size();
	size_t end = header.find("\r\n", pos);
	if (end == std::string::npos) {
		return "";
	}
	found = header.substr(pos, end - pos);
	return found;
}

std::map< std::string, std::string > parseHeader(const std::string &header)
{
	std::map< std::string, std::string > headers;

	if (header.empty()) {
		return headers;
	}

	std::string status_code = findHeaderElement(header, "Status:");
	std::string content_type = findHeaderElement(header, "Content-Type:");
	std::string content_length = findHeaderElement(header, "Content-Length:");
	std::string location = findHeaderElement(header, "Location:");
	// TODO : Add Cookie

	if (!status_code.empty()) {
		std::cout << "Status code found: " << status_code << "%" << std::endl;
		headers["Status"] = status_code;
	}
	else {
		headers["Status"] = "200 OK";
	}
	if (!content_type.empty()) {
		std::cout << "Content-Type found: " << content_type << std::endl;
		headers["Content-Type"] = content_type;
	}
	else {
		headers["Content-Type"] = "application/octet-stream";
	}
	if (!content_length.empty()) {
		headers["Content-Length"] = content_length;
	}
	if (!location.empty()) {
		headers["Location"] = location;
	}
	return headers;
}

std::string buildHeader(const std::string &cgiHeader)
{
	if (cgiHeader.empty()) {
		return "HTTP/1.1 200 OK\r\nContent-Type: "
			   "application/octet-stream\r\nTransfer-Encoding: chunked\r\n\r\n";
	}

	std::map< std::string, std::string > headers = parseHeader(cgiHeader);

	std::string header = "HTTP/1.1 ";

	header += headers["Status"];
	header += "\r\n";
	header += "Content-Type:" + headers["Content-Type"];
	header += "\r\n";
	if (headers.find("Content-Length") != headers.end()) {
		header += "Content-Length:" + headers["Content-Length"];
		header += "\r\n";
	}
	else {
		header += "Transfer-Encoding: chunked";
		header += "\r\n";
	}
	if (headers.find("Location") != headers.end()) {
		header += "Location:" + headers["Location"];
		header += "\r\n";
	}
	header += "\r\n";
	return header;
}

void adjustHeader(std::string *response)
{
	size_t		pos = response->find("\r\n\r\n");
	std::string header;
	std::string body;

	if (pos != std::string::npos) {
		std::cout << "Header found" << std::endl;
		header = response->substr(0, pos + 2);
		body = response->substr(pos + 4);
		std::cout << "Header: %" << header << "%" << std::endl;
		std::cout << "Body: %" << body << "%" << std::endl;
	}
	else {
		std::cout << "Header not found" << std::endl;
		header = "";
		body = *response;
	}

	std::string newHeader = buildHeader(header);

	*response = newHeader + body;

	std::cout << "New Response: " << *response << std::endl;
}