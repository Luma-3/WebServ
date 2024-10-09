/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/09 15:17:10 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

#include <iostream>

using client::Builder;
using std::map;
using std::string;

Builder::Builder() : _return_code("200") {}

Builder::Builder(const Builder &src) :
	_path(src._path),
	_url(src._url),
	_return_code(src._return_code)
{
}

Builder::~Builder() {}

Builder &Builder::operator=(const Builder &src)
{
	if (this == &src) {
		return *this;
	}
	_path = src._path;
	_url = src._url;
	_return_code = src._return_code;
	return *this;
}

void Builder::createUrlDefaultErrorPage()
{
	_url = string(DEFAULT_ERROR_PAGE) + ToString(_return_code) + ".html";
}

std::vector< char > Builder::createErrorPage()
{
	std::vector< char > body;

	std::string error_page = "<head><h1>ERROR " + _return_code +
							 findStatusMessage(_return_code) +
							 "Sorry for this ugly page bro </h1></head>";

	body.assign(error_page.begin(), error_page.end());

	return body;
}

const statement::Location *
Builder::findLocation(const std::string &requested_path)
{
	std::vector< const statement::Location * > locations;

	std::vector< const statement::Location * >::const_iterator it =
		_server->getLocations().begin();

	while (it != _server->getLocations().end()) {
		if ((*it)->getRoute() == requested_path) {
			return (*it);
		}
		++it;
	}
	return NULL;
}

std::vector< char > Builder::readDataRequest(std::ifstream &file)
{
	file.seekg(0, std::ios::end);

	size_t size = file.tellg();

	file.seekg(0, std::ios::beg);
	std::vector< char > body;
	body.resize(size);
	file.read(&body[0], static_cast< std::streamsize >(size));
	file.close();
	return body;
}

std::vector< char > Builder::getDataFromFileRequest(bool &key)
{
	while (true) {
		std::ifstream file(_url.c_str(), std::ios::binary);

		// if (_parser.getFileExtension() == "py") {
		// 	return handleCGI();
		// }
		if (file.is_open()) {
			return readDataRequest(file);
		}
		if (key) {
			createUrlDefaultErrorPage();
			key = false;
		}
		else {
			break;
		}
	}
	return createErrorPage();
}

const string &Builder::getResponse() const
{
	return _response;
}

// void Builder::setErrorPath(const string &code, const Parser &parser)
// {
// 	string new_path = parser.getConfigParam(code, P_ERRORPAGE);
// 	if (new_path.empty()) {
// 		_url = DEFAULT_ERROR_PAGE + code + ".html";
// 	}
// 	else {
// 		_url = new_path;
// 	}
// }

void Builder::accessRequestedFile(const Parser &parser)
{
if (_return_code == "200") {
	_url = parser.getPath() + parser.getFilename(); }

	if (access(_url.c_str(), F_OK) != 0) {
		if (errno == ENOENT) {
			_return_code = "404";
			// setErrorPath("404", parser);
		}
		else {
			_return_code = "402";
			// setErrorPath("402", parser);
		}
	}
}

void Builder::BuildResponse(client::Parser &parser, const Server *server,
							const Server *default_server)
{
	_server = server;
	(void)default_server;

	map< string, string > _headers = parser.getHeaders();
	bool				  key = true;
	_return_code = parser.getCodeResponse();

	accessRequestedFile(parser);

	std::vector< char > body = getDataFromFileRequest(key);

	_response =
		"HTTP/1.1 " + _return_code + findStatusMessage(_return_code) + "\r\n";
	_response +=
		"Content-Type: " + findContentType(parser.getFileExtension()) + "\r\n";

	_response += "Content-Length: " + ToString(body.size()) + "\r\n";

	if (_headers["Connection"] == "close") {
		_response += "Connection: close\r\n\r\n";
	}
	else {
		_response += "Connection: keep-alive\r\n\r\n";
	}
	_response += std::string(body.begin(), body.end());

	// std::cout << "Response : " << response << std::endl;
	reset();
}

void Builder::reset()
{
	_server = NULL;
	_path.clear();
	_url.clear();
	_return_code = "200";
}