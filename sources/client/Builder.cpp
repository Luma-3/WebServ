/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/11/04 16:24:44 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

#include <iostream>

// #include "cgi/CGI.hpp"

using client::Builder;
using std::string;

Builder::Builder(const VirtualServer  *server,
				 const VirtualServer  *default_server,
				 const client::Parser &parser) :
	_server(server),
	_default_server(default_server),
	_parser(parser)
{
	_request_path = parser.getRequestedPath();
	_filename = parser.getFilename();
	_code = parser.getCodeResponse();
	_extension = parser.getFileExtension();
	_connection_status = parser.getHeader("Connection");
}

void Builder::verifCGI(int &state)
{
	const Location *location = _server->getLocation(_request_path);
	std::string		root;

	if (location != NULL) {
		root = location->getRoot(_request_path);
		_path = root + _filename;
		if (access(_path.c_str(), F_OK | R_OK) != 0) {

			state = B_ERROR;
			_code = (errno == ENOENT) ? "404" : "403";
		}
		return;
	}
	root = _server->getRoot(_request_path);
	_path = root + _filename;
	if (access(_path.c_str(), F_OK | R_OK) != 0) {
		state = B_ERROR;
		_code = (errno == ENOENT) ? "404" : "403";
	}
}

void Builder::isCGI(int &state)
{
	static string cgi_extension[] = {"php", "py"};
	for (size_t i = 0; i < 3; ++i) {
		if (_extension == cgi_extension[i]) {
			state = CGI;
			verifCGI(state);
			return;
		}
	}
}

void Builder::createErrorPage()
{
	string error_page = DEFAULT_ERROR_PAGE;
	string title = "%@title@%";
	string message = "%@message@%";

	_extension = "html";
	error_page.replace(error_page.find(title), title.size(), "Error " + _code);
	error_page.replace(error_page.find(title), title.size(), "Error " + _code);
	error_page.replace(error_page.find(message), message.size(),
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

	if (_connection_status == "close") {
		response += "Connection: close\r\n\r\n";
	}
	else {
		response += "Connection: keep-alive\r\n\r\n";
	}

	response += std::string(_body.begin(), _body.end());
}

bool Builder::findErrorPageLocation()
{
	std::string error_page;
	std::string error_path;

	const Location *location = _server->getLocation(_request_path);

	if (location == NULL) {
		return false;
	}

	error_page = location->getParamValue(_code);
	error_path = location->getRoot(_request_path);

	if (error_page.empty()) {
		return false;
	}

	_path = error_path + error_page;
	_filename = error_page;
	_extension = Parser::findExtension(error_page);
	return true;
}

bool Builder::findErrorPageServer()
{
	std::string error_page = _server->getParamValue(_code);
	std::string error_path = _server->getParamValue("root");
	if (error_page.empty()) {
		return false;
	}

	_path = error_path + error_page;
	_filename = error_page;
	_extension = Parser::findExtension(error_page);
	return true;
}

bool Builder::findErrorPageDefaultServer()
{
	std::string error_page = _default_server->getParamValue(_code);
	std::string error_path = _default_server->getParamValue("root");

	if (error_page.empty()) {
		return false;
	}

	_path = error_path + error_page;
	_filename = error_page;
	_extension = Parser::findExtension(error_page);
	return true;
}

void Builder::findErrorPage()
{
	typedef bool (Builder::*ptr)(void);

	ptr tab[3] = {&Builder::findErrorPageLocation,
				  &Builder::findErrorPageServer,
				  &Builder::findErrorPageDefaultServer};

	for (size_t i = 0; i < 3; ++i) {
		if ((this->*tab[i])() && readDataRequest() == 0) {
			return;
		}
	}
	std::string error =
		(errno != 0) ? strerror(errno) : "No file found in config file";
	LOG_INFO("No Error page for code " + _code + ": " + error, _CSERVER);
	createErrorPage();
}

void Builder::verifMethod(int &state)
{
	if (_parser.getHeader("Method") == "DELETE") {
		state = DELETE;
		return;
	}

	std::vector< std::string > deny_methods;
	const Location			  *location = _server->getLocation(_request_path);
	if (location != NULL) {
		deny_methods = location->getParamList("deny_method");
	}
	else {
		deny_methods = _server->getParamList("deny_method");
	}
	if (deny_methods.empty()) {
		return;
	}
	for (size_t i = 0; i < deny_methods.size(); ++i) {
		if (deny_methods[i] == _parser.getHeader("Method")) {
			_code = "405";
			state = B_ERROR;
		}
	}
}

int Builder::readDataRequest()
{
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
		LOG_WARNING("Error Accessing file path: " + _path, _server);
		_code = (ret == ENOENT) ? "404" : "403";
		findErrorPage();
	}
}

void Builder::findFile()
{
	std::string root;
	if (_code == "200") {
		const Location *location = _server->getLocation(_request_path);

		if (location != NULL) {
			_path = location->getRoot(_request_path) + _filename;
		}
		else {
			_path = _server->getRoot(_request_path) + _filename;
		}
	}
	readFile();
}

void Builder::returnParam(int &state)
{
	const Location *location = _server->getLocation(_request_path);
	if (location != NULL) {
		if (!location->getParamPair("return").first.empty()) {
			_code = location->getParamPair("return").first;
			_location = location->getParamPair("return").second;
		}
	}
	else if (!_server->getParamPair("return").first.empty()) {
		_code = _server->getParamPair("return").first;
		_location = _server->getParamPair("return").second;
	}
	state = (_location.empty()) ? state : REDIRECT;
}

Builder::~Builder() {}