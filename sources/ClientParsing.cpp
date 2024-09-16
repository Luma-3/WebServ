/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientParsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:58:36 by anthony           #+#    #+#             */
/*   Updated: 2024/09/16 18:51:59 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

string Client::findContentType(const string &file_extension)
{

	if (file_extension == "html") return "text/html";
	if (file_extension == "css") return "text/css";
	if (file_extension == "js") return "text/javascript";
	if (file_extension == "jpg") return "image/jpeg";
	if (file_extension == "jpeg") return "image/jpeg";
	if (file_extension == "png")
		return "image/png";
	else
		return "text/plain";
}

void Client::createUrl(string &url)
{
	std::cout << "URL = " << url << std::endl;

	string url_path;
	string filename;
	string file_extension;

	size_t last_slash = url.find_last_of('/');
	size_t last_dot = url.find_last_of('.');

	if (last_slash == string::npos || last_slash == 0)
		url_path = "/";
	else {
		url_path = url.substr(0, url.find_last_of('/'));
	}

	if (last_dot == string::npos) {
		file_extension = "html";
	} else {
		file_extension = url.substr(last_dot + 1);
	}

	if (last_dot == string::npos || last_dot < last_slash) {
		filename = url.substr(last_slash + 1);
	} else {
		filename = url.substr(last_slash + 1, last_dot - last_slash - 1);
	}

	if (url_path.empty()) {
		url_path = "/";
		filename = "index";
		file_extension = "html";
	} else if (url_path[0] != '/') {
		url_path = "/" + url_path;
	}
	if (filename.empty()) filename = "index";
	if (file_extension.empty()) file_extension = "html";
	
	string root;
	if (_locations.find(url_path) == _locations.end()) {
		root = _locations["/"]["root"] + url_path + "/";
	} else {
		root = _locations[url_path]["root"] + "/";
	}
	_headers["Url"] = root + filename + "." + file_extension;

	_headers["Content-Type"] = findContentType(file_extension);
	
}

string Client::getAndErase(string &str, const string &delim)
{
	size_t pos = str.find(delim);
	string ret = str.substr(0, pos);

	if (pos == string::npos) return ret;
	str.erase(0, pos + delim.length());
	return ret;
}

void Client::getHeaderFromRequest(const size_t &line_break_pos) {

	string line = _buffer.substr(0, line_break_pos);

	_headers["Method"] = getAndErase(line, " ");
	_headers["Url"] = getAndErase(line, " ");
	_headers["httpVersion"] = getAndErase(line, "\n");

	std::cout << "URL avant parsing : " << _headers["Url"] << std::endl;
	createUrl(_headers["Url"]);

	std::cout << "URL apres parsing : " << _headers["Url"] << std::endl;
	
	_buffer.erase(0, line_break_pos + 2);
	_haveHeader = true;
}

void Client::getBodyFromRequest(size_t &line_break_pos) {
	
	string line;
	string key;
	string value;

	while ((line_break_pos = _buffer.find("\r\n")) != string::npos) {
		
		line = _buffer.substr(0, line_break_pos);
		if (line.empty()) break;

		key = getAndErase(line, ": ");
		value = line;
		
		_headers[key] = value;
		_buffer.erase(0, line_break_pos + 2);
	}
}

void Client::parseRequest(void *buff)
{
	size_t line_break_pos = 0;
	string line;

	_buffer += (char *)buff;
	line_break_pos = _buffer.find("\r\n");

	if (line_break_pos == string::npos) return;

	if (!_haveHeader) {
		getHeaderFromRequest(line_break_pos);
	}
	
	getBodyFromRequest(line_break_pos);
	
	if (!checkRequest()) changeUrlError();
}

bool	Client::checkRequest() {

	string method = _headers["Method"];
	string url = _headers["Url"];

	if (method != "GET" && method != "POST" && method != "DELETE") { 
		_codeResponse = "405";
		return false;
	}	
	if (method.empty() ||
		_headers["Url"].empty() ||
		_headers["httpVersion"].empty() ||
		_headers["Host"].empty()) {
		_codeResponse = "400";
		return false;
	}
	if (_headers["httpVersion"] != "HTTP/1.1") {
		_codeResponse = "400";
		return false;
	}
	std::cout << "URl tester avec access : " << url << std::endl;
	if (access(url.c_str(), F_OK | R_OK) != 0) {
		if (errno == ENOENT) _codeResponse = "404";
		else _codeResponse = "403";
		return false;
	}
	return true;
}

void	Client::changeUrlError() {

	_headers["Url"] = _locations["/" + _codeResponse]["root"];
}
