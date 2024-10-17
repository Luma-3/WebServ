/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/17 17:57:31 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

#include <iostream>

using client::Builder;
using std::map;
using std::string;

Builder::Builder(const VirtualServer *server,
				 const VirtualServer *default_server) :
	_server(server),
	_default_server(default_server)
{
}

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
	_content_type = "text/html";

	body.assign(error_page.begin(), error_page.end());
}

void Builder::buildHeader(const Parser		&parser,
						  const std::string &location_param, int body_size)
{
	std::map< string, string > headers = parser.getHeaders();
	string					   code_message = findStatusMessage(_code);

	_response = "HTTP/1.1 " + _code + code_message + "\r\n";

	if (!location_param.empty()) {
		_response += "Location: " + location_param + "\r\n";
	}
	else {
		_response += "Content-Type: " + _content_type + "\r\n";
	}
	_response += "Content-Length: " + ToString(body_size) + "\r\n";

	if (headers["Connection"] == "close") {
		_response += "Connection: close\r\n\r\n";
	}
	else {
		_response += "Connection: keep-alive\r\n\r\n";
	}
	std::cout << "response : " << _response << std::endl;
}

int Builder::findErrorpageLocationServer(const VirtualServer *server,
										 const std::string	 &code,
										 std::vector< char > &body,
										 const std::string	 &path)
{
	const Location *location;
	std::string		error_page;
	std::string		error_path;

	location = server->getLocation(path);
	if (location != NULL) {
		error_page = location->getParamValue(code);
		error_path = location->getParamValue("root");
		if (!error_page.empty() &&
			readDataRequest(body, error_path + error_page) == 0) {
			return 0;
		}
	}
	error_page = server->getParamValue(code);
	if (error_page.empty()) {
		return -1;
	}
	error_path = server->getParamValue("root");
	if (readDataRequest(body, error_path + error_page) == 0) {
		return 0;
	}
	return -1;
}

void Builder::findBodyErrorPage(const client::Parser &parser,
								std::vector< char >	 &body)
{

	if (findErrorpageLocationServer(_server, _code, body,
									parser.getRequestedPath()) == 0) {
		return;
	}
	if (findErrorpageLocationServer(_default_server, _code, body,
									parser.getRequestedPath()) == 0) {
		return;
	}
	createErrorPage(_code, body);
}

int Builder::readDataRequest(std::vector< char > &body,
							 const std::string	 &filePath)

{
	if (access(filePath.c_str(), F_OK | R_OK) != 0) {
		return errno;
	}
	size_t pos = filePath.find_last_of('.');
	if (pos == std::string::npos) {
		_content_type = "application/octet-stream";
	}
	else {
		_content_type = findContentType(filePath.substr(pos + 1));
	}

	std::ifstream file(filePath.c_str(), std::ios::binary);
	file.seekg(0, std::ios::end);

	size_t size = file.tellg();

	file.seekg(0, std::ios::beg);
	body.resize(size);
	file.read(&body[0], static_cast< std::streamsize >(size));
	file.close();
	return 0;
}

void Builder::readFile(const client::Parser &parser, const std::string &path,
					   std::vector< char > &body)
{
	int ret;

	ret = readDataRequest(body, path);
	if (ret != 0) {
		_code = (ret == ENOENT) ? "404" : "403";
		findBodyErrorPage(parser, body);
	}
}

void Builder::findFile(const client::Parser &parser, std::vector< char > &body)
{
	std::string root;

	std::string path = parser.getRequestedPath() + parser.getFilename();

	const Location *location = _server->getLocation(path);
	if (location != NULL) {
		root = location->getRoot(path);
	}
	else {
		root = _server->getRoot(path);
	}
	readFile(parser, root, body);
}

bool Builder::returnParam(client::Parser &parser)
{
	std::string		path = parser.getRequestedPath() + parser.getFilename();
	const Location *location = _server->getLocation(path);
	if (location != NULL) {
		if (location->getParamPair("return").first.empty() == false) {
			_code = location->getParamPair("return").first;
			parser.setPath(location->getParamPair("return").second);
			return true;
		}
		return false;
	}
	else if (_server->getParamPair("return").first.empty() == false) {
		_code = _server->getParamPair("return").first;
		parser.setPath(_server->getParamPair("return").second);
		return true;
	}
	return false;
}

void Builder::BuildResponse(client::Parser &parser)
{
	std::vector< char > body;
	_code = parser.getCodeResponse();

	if (_code != "200") {
		findBodyErrorPage(parser, body);
	}
	else if (returnParam(parser)) {
		buildHeader(parser, parser.getPath(), 0);
		reset();
		return;
	}
	else if (isDirRequest(parser.getRequestedPath() + parser.getFilename())) {
		indexOrAutoindexList(parser, body);
	}
	else {
		findFile(parser, body);
	}
	buildHeader(parser, "", body.size());
	_response += std::string(body.begin(), body.end());
	reset();
}
void Builder::reset()
{
	_path.clear();
	_final_url.clear();
	_code = "200";
}