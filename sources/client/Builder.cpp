/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/11/26 09:53:00 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

#include <iostream>

#include "finder.hpp"

using client::Builder;
using std::string;

Builder::Builder(const VirtualServer *server, const client::Parser &parser) :
	_server(server),
	_parser(parser),
	_requestPath(parser.getRequestedPath()),
	_path(parser.getRequestedPath() + parser.getFilename()),
	_filename(parser.getFilename()),
	_extension(parser.getFileExtension()),
	_connectionStatus(parser.getHeader("Connection")),
	_code(parser.getCodeResponse())
{
}

void Builder::verifCGI(int &state)
{
	const std::string root = findRoot(_requestPath, _server);

	_path = root + _filename;

	if (access(_path.c_str(), F_OK | R_OK) != 0) {
		state = B_ERROR;
		_code = (errno == ENOENT) ? "404" : "403";
	}
}

void Builder::isCGI(int &state)
{
	const static string cgi_extension[] = {"php", "py"};
	for (size_t i = 0; i < 2; ++i) {
		if (_extension == cgi_extension[i]) {
			state = CGI;
			verifCGI(state);
			return;
		}
	}
}

void Builder::createErrorPage()
{
	string		 error_page = DEFAULT_ERROR_PAGE;
	const string title = "%@title@%";
	const string message = "%@message@%";

	_extension = "html";
	error_page.replace(error_page.find(title), title.size(), "Error " + _code);
	error_page.replace(error_page.find(title), title.size(), "Error " + _code);
	error_page.replace(error_page.find(message), message.size(),
					   findStatusMessage(_code));
	_body.assign(error_page.begin(), error_page.end());
}

void Builder::BuildResponse(string &response)
{
	const string code_message = findStatusMessage(_code);
	const string content_type = findContentType(_extension);

	response = "HTTP/1.1 " + _code + code_message + "\r\n";

	response += (!_returnLocation.empty())
				  ? "Location: " + _returnLocation + "\r\n"
				  : "Content-Type: " + content_type + "\r\n";

	response += "Content-Length: " + ToString(_body.size()) + "\r\n";

	response += (_connectionStatus == "close")
				  ? "Connection: close\r\n\r\n"
				  : "Connection: keep-alive\r\n\r\n";

	response += std::string(_body.begin(), _body.end());
}

void Builder::findErrorPage()
{
	std::string path_error_page;

	path_error_page = findErrorParam(_code, _requestPath, _server);

	if (!path_error_page.empty()) {
		_path = path_error_page;
		_filename = Parser::findFilename(path_error_page);
		_extension = Parser::findExtension(_filename);
	}

	if (path_error_page.empty() || readDataRequest() != 0) {
		LOG_INFO(
			"No Error page for code " + _code + ": " +
			((errno != 0) ? strerror(errno) : "No file found in config file"));
		createErrorPage();
	}
}

bool Builder::isAcceptedMethod(int &state, const std::string &current_method)
{
	std::vector< std::string > methods;

	methods = findParamList("method", _requestPath, _server);
	if (methods.empty()) {
		return true;
	}
	for (size_t i = 0; i < methods.size(); ++i) {
		if (methods[i] == current_method) {
			return true;
		}
	}
	_code = "405";
	state = B_ERROR;
	return false;
}

void Builder::isDelete(int &state)
{
	const std::string current_method = _parser.getHeader("Method");

	if (current_method == "DELETE") {
		state = DELETE;
		return;
	}
}

void Builder::verifyMethodsAndBodySize(int &state)
{
	const std::string current_method = _parser.getHeader("Method");
	const std::string max_body_size =
		findParam("max_body_size", _requestPath, _server);

	if (!isAcceptedMethod(state, current_method)) {
		return;
	}

	if (max_body_size.empty()) {
		return;
	}
	if (current_method == "POST") {
		const size_t body_size = _parser.getHeader("body").size();
		if (static_cast< int >(body_size) > atoi(max_body_size.c_str())) {
			_code = "413";
			state = B_ERROR;
			return;
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

	const size_t size = file.tellg();

	file.seekg(0, std::ios::beg);
	_body.resize(size);
	if (file.read(&_body[0], static_cast< std::streamsize >(size))) {
		file.close();
		return 0;
	}
	file.close();
	return FAILURE;
}

void Builder::readFile()
{
	const int ret = readDataRequest();

	if (ret != 0) {
		LOG_WARNING("Error Accessing file path: " + _path);
		_code = (ret == ENOENT) ? "404" : "403";
		findErrorPage();
	}
}

void Builder::findFile()
{
	std::string root;
	if (_code == "200") {
		root = findRoot(_requestPath, _server);
		_path = root + _filename;
	}
	readFile();
}

void Builder::returnParam(int &state)
{
	const Location *location = _server->getLocation(_requestPath);
	if (location != NULL) {
		if (!location->getParamPair("return").first.empty()) {
			_code = location->getParamPair("return").first;
			_returnLocation = location->getParamPair("return").second;
		}
	}
	else if (!_server->getParamPair("return").first.empty()) {
		_code = _server->getParamPair("return").first;
		_returnLocation = _server->getParamPair("return").second;
	}
	state = (_returnLocation.empty()) ? state : REDIRECT;
}

Builder::~Builder() {}