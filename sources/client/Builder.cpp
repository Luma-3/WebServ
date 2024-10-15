/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/15 19:01:51 by anthony          ###   ########.fr       */
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

void Builder::findIndex(const client::Parser &parser, std::vector< char > &body)
{
	std::string		path = parser.getRequestedPath();
	const Location *location = _server->getLocation(path);

	std::string index;
	std::string autoindex;
	std::string root;

	if (location != NULL) {
		index = location->getParamValue("index");
		root = location->getParamValue("root");
		if (!index.empty()) {
			return readFile(parser, root + index, body);
		}
		autoindex = location->getParamValue("autoindex");
		if (!autoindex.empty()) {
			returnAutoindexList(parser, body,
								(autoindex == "on" ? true : false));
			return;
		}
	}
	index = _server->getParamValue("index");
	root = _server->getParamValue("root");
	if (!index.empty()) {
		return readFile(parser, root + index, body);
	}
	autoindex = _server->getParamValue("autoindex");
	if (!autoindex.empty()) {
		if (root.empty() || _autoindex == true) {
			root = parser.getRequestedPath();
		}

		returnAutoindexList(parser, body, autoindex == "on" ? true : false);
		return;
	}
	_code = "403";
	findBodyErrorPage(parser, body);
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
	std::string		path = parser.getRequestedPath();
	const Location *location = _server->getLocation(path);
	std::string		file = parser.getFilename();
	std::cout << "Je cherche le fichier : " << file << std::endl;

	std::string root;

	if (location != NULL) {
		root = location->getParamValue("root");
		if (root.empty()) {
			return readFile(parser, path + file, body);
		}
		return readFile(parser, root + file, body);
	}
	root = _server->getParamValue("root");
	if (root.empty()) {
		return readFile(parser, path + file, body);
	}
	readFile(parser, root + file, body);
}

bool Builder::returnParam(client::Parser &parser)
{
	std::string		path = parser.getRequestedPath();
	const Location *location = _server->getLocation(path);
	if (location != NULL) {
		if (location->getParamPair("return").first.empty()) {
			return false;
		}
		_code = location->getParamPair("return").first;
		parser.setPath(location->getParamPair("return").second);
		return true;
	}
	else if (_server->getParamPair("return").first.empty()) {
		return false;
	}
	_code = _server->getParamPair("return").first;
	parser.setPath(_server->getParamPair("return").second);
	return true;
}

std::string getBeforeLastSlash(const std::string &path)
{
	size_t lastSlashPos = path.rfind('/');
	if (lastSlashPos == std::string::npos) {
		return path;
	}
	size_t secondLastSlashPos = path.rfind('/', lastSlashPos - 1);
	if (secondLastSlashPos == std::string::npos) {
		return path.substr(0, lastSlashPos);
	}
	return path.substr(0, secondLastSlashPos);
}

void Builder::tambouille(std::string &path)
{
	size_t pos = path.find_first_of("/");
	if (pos == std::string::npos || pos == path.size() - 1) {
		return;
	}
	path = path.substr(pos + 1);
}

void Builder::returnAutoindexList(const client::Parser &parser,
								  std::vector< char > &body, bool autoindex)
{
	if (autoindex == false) {
		_code = "403";
		findBodyErrorPage(parser, body);
		return;
	}

	std::string new_path;
	std::string tmp;

	const Location *location = _server->getLocation(parser.getRequestedPath());
	if (location != NULL) {
		new_path = location->getParamValue("root");
		if (new_path.empty()) {
			new_path = parser.getRequestedPath() + parser.getFilename();
		}
		else if (parser.getRequestedPath() != "/") {
			new_path += parser.getRequestedPath() + parser.getFilename();
		}
	}
	else {
		new_path = _server->getParamValue("root");
		if (new_path.empty()) {
			new_path = parser.getRequestedPath() + parser.getFilename();
		}
		else {
			new_path += parser.getRequestedPath() + parser.getFilename();
		}
	}
	if (new_path.length() != 1 && new_path[0] == '/') {
		new_path = new_path.substr(1);
	}
	if (new_path[new_path.size() - 1] != '/') {
		new_path += "/";
	}

	std::cout << "New path : " << new_path << std::endl;
	int ret = access(new_path.c_str(), F_OK);
	if (ret != 0) {
		_code = (errno == ENOENT) ? "404" : "403";
		std::cout << "Le code est : " << _code << std::endl;
		findBodyErrorPage(parser, body);
		return;
	}
	DIR *dir = opendir(new_path.c_str());
	if (dir == NULL) {
		if (errno == ENOENT) {
			_code = "404";
		}
		else {
			_code = "403";
		}
		findBodyErrorPage(parser, body);
		return;
	}

	tmp = DEFAULT_AUTOINDEX_PAGE_HEAD;
	body.insert(body.end(), tmp.begin(), tmp.end());

	tmp = DEFAULT_AUTOINDEX_RETURN_BUTTON;
	std::string back_path = new_path.substr(0, new_path.find_last_of('/'));
	if (back_path[back_path.size() - 1] != '/') {
		back_path = back_path.substr(0, back_path.size() - 1);
	}
	back_path = back_path.substr(0, back_path.find_last_of('/'));
	if (back_path[back_path.size() - 1] != '/') {
		back_path = "/";
	}
	std::cout << "Mon back path est : " << back_path << std::endl;
	tmp.replace(tmp.find("%@return_path@%"), 15, back_path);
	body.insert(body.end(), tmp.begin(), tmp.end());

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		std::string html_line_file = DEFAULT_AUTOINDEX_LIST;
		if (entry->d_name[0] == '.') {
			continue;
		}
		std::string relative_path = new_path;
		if (relative_path[relative_path.size() - 1] != '/') {
			relative_path += "/";
		}
		tambouille(relative_path);
		relative_path += entry->d_name;
		std::cout << "Relative path : " << relative_path << std::endl;
		html_line_file.replace(html_line_file.find("%@file@%"), 8,
							   relative_path);
		html_line_file.replace(html_line_file.find("%@file@%"), 8,
							   entry->d_name);
		body.insert(body.end(), html_line_file.begin(), html_line_file.end());
	}
	closedir(dir);
	std::string foot = DEFAULT_AUTOINDEX_PAGE_FOOT;
	body.insert(body.end(), foot.begin(), foot.end());
	_content_type = "text/html";
	_autoindex = true;
}

void Builder::BuildResponse(client::Parser &parser, bool autoindex)
{
	std::vector< char > body;
	_code = parser.getCodeResponse();

	if (autoindex == true) {
		returnAutoindexList(parser, body, autoindex);
	}
	else if (_code != "200") {
		findBodyErrorPage(parser, body);
	}
	else if (returnParam(parser) == true) {
		buildHeader(parser, parser.getPath(), 0);
		reset();
		return;
	}
	else if (parser.getFilename().empty()) {
		findIndex(parser, body);
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