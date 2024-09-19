/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:58:36 by anthony           #+#    #+#             */
/*   Updated: 2024/09/19 14:18:19 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

client::Parser::Parser()
{
	_haveHeader = false;
	_codeResponse = "200";
}

client::Parser::~Parser() {}

string client::Parser::findContentType(const string &file_extension)
{
	map< string, string > content_type_list;
	content_type_list["html"] = "text/html";
	content_type_list["css"] = "text/css";
	content_type_list["js"] = "text/javascript";
	content_type_list["jpg"] = "image/jpeg";
	content_type_list["jpeg"] = "image/jpeg";
	content_type_list["png"] = "image/png";
	content_type_list["gif"] = "image/gif";
	content_type_list["bmp"] = "image/bmp";
	content_type_list["ico"] = "image/x-icon";
	content_type_list["svg"] = "image/svg+xml";
	content_type_list["json"] = "application/json";
	content_type_list["pdf"] = "application/pdf";
	content_type_list["zip"] = "application/zip";
	content_type_list["tar"] = "application/x-tar";

	if (content_type_list.find(file_extension) == content_type_list.end()) {
		return "text/plain";
	}
	return content_type_list[file_extension];
}

void client::Parser::setDefaultUrl(string &url, string &filename,
								   string &file_extension)
{
	url = "/";
	filename = "index";
	file_extension = "html";
}

void client::Parser::createUrl(string &url)
{
	string url_path;
	string filename;
	string file_extension;

	size_t last_slash = url.find_last_of('/');
	size_t last_dot = url.find_last_of('.');

	// If there is no slash, the path is the root
	if (last_slash == string::npos || !last_slash) {
		url_path = "/";
	} else {
		url_path = url.substr(0, last_slash);
	}

	// If the path is empty, it is the root
	if (url_path.empty()) {
		setDefaultUrl(url_path, filename, file_extension);
	} else if (url_path[0] != '/') {
		url_path = "/" + url_path;
	}

	// If there is no dot, the file extension is html
	if (last_dot == string::npos) {
		file_extension = "html";
	} else {
		file_extension = url.substr(last_dot + 1);
	}
	if (file_extension.empty()) {
		file_extension = "html";
	}

	// If there is no dot or the dot is before the last slash, the filename
	// is the last part of the url
	if (last_dot == string::npos || last_dot < last_slash) {
		filename = url.substr(last_slash + 1);
	} else {
		filename = url.substr(last_slash + 1, last_dot - last_slash - 1);
	}
	if (filename.empty()) {
		filename = "index";
	}

	string root;
	root = "/";
	// if (_locations.find(url_path) == _locations.end()) {
	// 	root = _locations["/"]["root"] + url_path + "/";
	// } else {
	// 	root = _locations[url_path]["root"] + "/";
	// }
	_headers["Url"] = root + filename + "." + file_extension;

	_headers["Content-Type"] = findContentType(file_extension);
}

string client::Parser::getAndErase(string &str, const string &delim)
{
	size_t pos = str.find(delim);
	string ret = str.substr(0, pos);

	if (pos == string::npos) {
		return ret;
	}
	str.erase(0, pos + delim.length());
	return ret;
}

void client::Parser::getHeaderFromRequest(const size_t &line_break_pos)
{
	string line = _buffer.substr(0, line_break_pos);

	_headers["Method"] = getAndErase(line, " ");
	_headers["Url"] = getAndErase(line, " ");
	_headers["httpVersion"] = getAndErase(line, "\n");

	createUrl(_headers["Url"]);

	_buffer.erase(0, line_break_pos + 2);
	_haveHeader = true;
}

void client::Parser::getBodyFromRequest(size_t &line_break_pos)
{
	string line;
	string key;
	string value;

	while ((line_break_pos = _buffer.find("\r\n")) != string::npos) {

		line = _buffer.substr(0, line_break_pos);
		if (line.empty()) {
			break;
		}

		key = getAndErase(line, ": ");
		value = line;

		_headers[key] = value;
		_buffer.erase(0, line_break_pos + 2);
	}
}

bool client::Parser::InvalidMethod()
{
	string method = _headers["Method"];
	if (method != "GET" && method != "POST" && method != "DELETE") {
		_codeResponse = "405";
		return true;
	}
	return false;
}

bool client::Parser::InvalidHeader()
{
	if (_headers["Method"].empty() || _headers["Url"].empty() ||
		_headers["httpVersion"].empty() || _headers["Host"].empty() ||
		_headers["httpVersion"] != "HTTP/1.1") {
		_codeResponse = "400";
		return true;
	}
	return false;
}
bool client::Parser::checkRequest()
{

	string method = _headers["Method"];
	string url = _headers["Url"];
	bool   error = false;

	if (InvalidMethod() || InvalidHeader()) {
		error = true;
	} else if (access(url.c_str(), F_OK) != 0) {
		error = true;
		if (errno == ENOENT) {
			// NOT FOUND
			_codeResponse = "404";
		} else {
			// NO PERMISSION
			_codeResponse = "403";
		}
	}
	return error;
}

void client::Parser::changeUrlError()
{
	// _headers["Url"] = _locations["/" + _codeResponse]["root"];
}

void client::Parser::parseRequest(void *buff)
{
	size_t line_break_pos = 0;
	string line;

	_buffer += static_cast< char * >(buff);
	line_break_pos = _buffer.find("\r\n");

	if (line_break_pos == string::npos) {
		return;
	}

	if (!_haveHeader) {
		getHeaderFromRequest(line_break_pos);
	}

	getBodyFromRequest(line_break_pos);

	if (checkRequest()) {
		changeUrlError();
	}
}