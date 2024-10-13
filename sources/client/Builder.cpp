/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/13 00:01:02 by jbrousse         ###   ########.fr       */
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

void Builder::createErrorPage(const std::string	  &return_code,
							  std::vector< char > &body)
{
	string error_page = DEFAULT_ERROR_PAGE;

	error_page.replace(error_page.find("%@title@%"), 9, "Error " + return_code);
	error_page.replace(error_page.find("%@title@%"), 9, "Error " + return_code);
	error_page.replace(error_page.find("%@message@%"), 11,
					   findStatusMessage(return_code));

	body.assign(error_page.begin(), error_page.end());
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
	string error_page = DEFAULT_ERROR_PAGE;
	std::cout << error_page << std::endl;
	error_page.replace(error_page.find("%@title@%"), 9,
					   "Error " + parser.getCodeResponse());
	error_page.replace(error_page.find("%@title@%"), 9,
					   "Error " + parser.getCodeResponse());
	error_page.replace(error_page.find("%@message@%"), 11,
					   findStatusMessage(parser.getCodeResponse()));
}

void Builder::findErrorPath(Parser &parser)
{
	s_info_param info;

	// bool found_param = parser.getConfigParam(info, F_ERRORPAGE, F_ROOT);
	bool found_param = false;
	if (found_param && !info.error_page.empty()) {
		std::cout << "J;ai trouve un error page a cette root : " << info.root
				  << std::endl;
		parser.setPathAndFilename(info.root, info.error_page);
		_final_url = parser.getPath() + parser.getFilename();
		if (access(_final_url.c_str(), F_OK | R_OK) == 0) {
			return;
		}
	}
	_final_url = "";
}
void Builder::accessRequestedFile(Parser &parser)
{
	if (parser.getCodeResponse() != "200") {
		findErrorPath(parser);
		return;
	}
	_final_url = parser.getPath() + parser.getFilename();
	if (access(_final_url.c_str(), F_OK | R_OK) != 0) {
		if (errno == ENOENT) {
			std::cout << "code 404" << std::endl;
			parser.setCodeResponse("404");
			findErrorPath(parser);
		}
		else {
			std::cout << "code 402" << std::endl;
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

void Builder::BuildResponse(client::Parser &parser)
{
	std::vector< char > body;

	accessRequestedFile(parser);
	if (_final_url.empty()) {
		createErrorPage(parser.getCodeResponse(), body);
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
	_path.clear();
	_final_url.clear();
}