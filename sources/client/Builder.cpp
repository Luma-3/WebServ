/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/10 15:43:09 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

#include <iostream>

using client::Builder;
using std::map;
using std::string;

Builder::Builder() {}

Builder::Builder(const Builder &src) :
	_path(src._path),
	_final_url(src._final_url),
	_response(src._response)
{
}

Builder::~Builder() {}

Builder &Builder::operator=(const Builder &src)
{
	if (this == &src) {
		return *this;
	}
	_path = src._path;
	_final_url = src._final_url;
	_response = src._response;
	return *this;
}

const string &Builder::getResponse() const
{
	return _response;
}

std::vector< char > Builder::createErrorPage(const std::string &return_code)
{
	std::vector< char > body;

	std::string error_page = "<head><h1>ERROR " +
							 findStatusMessage(return_code) +
							 "Sorry for this ugly page bro </h1></head>";

	body.assign(error_page.begin(), error_page.end());

	return body;
}

std::vector< char > Builder::readDataRequest()
{
	std::ifstream file(_final_url.c_str(), std::ios::binary);
	file.seekg(0, std::ios::end);

	size_t size = file.tellg();

	file.seekg(0, std::ios::beg);
	std::vector< char > body;
	body.resize(size);
	file.read(&body[0], static_cast< std::streamsize >(size));
	file.close();
	return body;
}

void Builder::findDefaultErrorPath(Parser &parser)
{
	parser.setPathAndFilename(DEFAULT_ERROR_PAGE,
							  "error" + parser.getCodeResponse() + ".html");
	_final_url = parser.getPath() + parser.getFilename();
	if (access(_final_url.c_str(), F_OK) != 0) {
		_final_url = "";
		parser.setPathAndFilename("", ".html");
	}
}

void Builder::findErrorPath(Parser &parser)
{
	std::pair< string, string > error_page =
		parser.getConfigParam(parser.getRequestedPath(), P_ERRORPAGE);

	if (error_page.first.empty()) {
		findDefaultErrorPath(parser);
	}
	else {
		parser.setPathAndFilename(error_page.second, error_page.first);
		_final_url = parser.getPath() + parser.getFilename();
		if (access(_final_url.c_str(), F_OK) != 0) {
			findDefaultErrorPath(parser);
		}
	}
}
void Builder::accessRequestedFile(Parser &parser)
{
	if (parser.getCodeResponse() != "200") {
		findErrorPath(parser);
	}
	_final_url = parser.getPath() + parser.getFilename();
	if (access(_final_url.c_str(), F_OK) != 0) {
		if (errno == ENOENT) {
			parser.setCodeResponse("404");
			findErrorPath(parser);
		}
		else {
			parser.setCodeResponse("402");
			findErrorPath(parser);
		}
	}
}

void Builder::buildHeader(const Parser &parser, std::vector< char > &body)
{
	std::map< string, string > headers = parser.getHeaders();
	string					   code = parser.getCodeResponse();
	string					   state = findStatusMessage(code);
	string content_type = findContentType(parser.getFileExtension());

	_response = "HTTP/1.1 " + code + state + "\r\n";

	_response += "Content-Type: " + content_type + "\r\n";

	_response += "Content-Length: " + ToString(body.size()) + "\r\n";

	if (headers["Connection"] == "close") {
		_response += "Connection: close\r\n\r\n";
	}
	else {
		_response += "Connection: keep-alive\r\n\r\n";
	}
}

void Builder::BuildResponse(client::Parser &parser, const Server *server,
							const Server *default_server)
{
	std::vector< char > body;

	_server = server;
	(void)default_server;

	accessRequestedFile(parser);
	if (_final_url.empty()) {
		body = createErrorPage(parser.getCodeResponse());
	}
	else
		body = readDataRequest();
	buildHeader(parser, body);

	if (parser.getHeaders().at("Method") == "GET" ||
		parser.getCodeResponse() != "200") {
		_response += std::string(body.begin(), body.end());
	}
	// TODO POST and DELETE
	reset();
}

void Builder::reset()
{
	_server = NULL;
	_path.clear();
	_final_url.clear();
}