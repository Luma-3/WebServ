/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/19 17:42:42 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

#include <iostream>

#include "cgi/CGI.hpp"

using client::Builder;
using std::map;
using std::string;

Builder::Builder(const VirtualServer  *server,
				 const VirtualServer  *default_server,
				 const client::Parser &parser) :
	_server(server),
	_default_server(default_server),
	_parser(parser)
{
	_request_path = parser.getRequestedPath();
	std::cout << "Request path: " << _request_path << std::endl;
	_filename = parser.getFilename();
	_code = parser.getCodeResponse();
	_extension = parser.getFileExtension();
}

void Builder::createErrorPage()
{
	string error_page = DEFAULT_ERROR_PAGE;

	_extension = "html";
	error_page.replace(error_page.find("%@title@%"), 9, "Error " + _code);
	error_page.replace(error_page.find("%@title@%"), 9, "Error " + _code);
	error_page.replace(error_page.find("%@message@%"), 11,
					   findStatusMessage(_code));

	_body.assign(error_page.begin(), error_page.end());
}

void Builder::BuildResponse(string &response)
{
	string code_message = findStatusMessage(_code);
	string content_type = findContentType(_extension);

	response = "HTTP/1.1 " + _code + code_message + "\r\n";

	if (!_location.empty()) {
		response += "Location: " + _location + "\r\n";
	}
	else {
		response += "Content-Type: " + content_type + "\r\n";
	}
	response += "Content-Length: " + ToString(_body.size()) + "\r\n";

	if (_parser.getHeader("Connection") == "close") {
		response += "Connection: close\r\n\r\n";
	}
	else {
		response += "Connection: keep-alive\r\n\r\n";
	}

	response += std::string(_body.begin(), _body.end());
}

void Builder::findErrorpageLocationServer()
{
	const Location *location;
	std::string		error_page;
	std::string		error_path;

	location = _server->getLocation(_request_path);
	if (location == NULL) {
		error_page = _server->getParamValue(_code);
		if (!error_page.empty()) {
			error_path = _server->getRoot(_request_path);
		}
		else {
			error_page = _default_server->getParamValue(_code);
			error_path = _default_server->getRoot(_request_path);
		}
		_path = error_path + error_page;
		_filename = error_page;
		_extension = Parser::findExtension(error_page);
		return;
	}
	else {
		error_page = location->getParamValue(_code);
		error_path = location->getRoot(_request_path);
		_path = error_path + error_page;
		_filename = error_page;
		_extension = Parser::findExtension(error_page);
		if (!error_page.empty()) {
			return;
		}
	}
	createErrorPage();
}

int Builder::readDataRequest()
{
	std::cout << "Accessing: " << _path << std::endl;

	if (access(_path.c_str(), F_OK | R_OK) != 0) {
		return errno;
	}

	std::ifstream file(_path.c_str(), std::ios::binary);
	file.seekg(0, std::ios::end);

	size_t size = file.tellg();

	file.seekg(0, std::ios::beg);
	_body.resize(size);
	file.read(&_body[0], static_cast< std::streamsize >(size));
	file.close();
	return 0;
}

void Builder::readFile()
{
	int ret = readDataRequest();

	if (ret != 0) {
		_code = (ret == ENOENT) ? "404" : "403";
		findErrorpageLocationServer();
	}
}

void Builder::findFile()
{
	std::string root;
	if (_code == "200") {
		std::cout << "JE SUIS DANS FIND FILE" << std::endl;
		const Location *location = _server->getLocation(_request_path);
		std::cout << _request_path << std::endl;
		if (location != NULL) {
			_path = location->getRoot(_request_path) + _filename;
			std::cout << "Path2: " << _path << std::endl;
		}
		else {
			_path = _server->getRoot(_request_path) + _filename;
		}
		std::cout << "Path: " << _path << std::endl;
	}
	readFile();
}

void Builder::returnParam()
{
	const Location *location = _server->getLocation(_request_path);
	if (location != NULL) {
		if (location->getParamPair("return").first.empty() == false) {
			_code = location->getParamPair("return").first;
			_location = location->getParamPair("return").second;
		}
	}
	else if (_server->getParamPair("return").first.empty() == false) {
		_code = _server->getParamPair("return").first;
		_location = _server->getParamPair("return").second;
	}
}

Builder::~Builder() {}