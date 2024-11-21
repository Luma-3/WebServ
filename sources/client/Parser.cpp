/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:58:36 by anthony           #+#    #+#             */
/*   Updated: 2024/11/21 10:37:39 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using client::Parser;
using std::string;

Parser::Parser() : _codeResponse("200")
{
	_headers["Connection"] = "keep-alive";
}

bool Parser::InvalidMethod()
{
	const string method = _headers["Method"];

	if (method != "GET" && method != "POST" && method != "DELETE") {
		_codeResponse = "405";
		return true;
	}
	return false;
}

bool Parser::InvalidHeader()
{
	if (_headers["Method"].empty() || _headers["Url"].empty()) {
		_codeResponse = "400";
		return true;
	}
	if (_headers["httpVersion"] != "HTTP/1.1" ||
		_headers["httpVersion"].empty()) {
		_codeResponse = "505";
		return true;
	}
	return false;
}

void Parser::getBodyFromRequest()
{
	if (_buffer.empty()) {
		return;
	}

	if (!_buffer.empty() && _headers["Method"] == "POST") {

		if (_buffer[0] == '\r' && _buffer[1] == '\n') {
			_buffer.erase(0, 2);
		}
		_headers["body"] = _buffer;
	}
}

void Parser::getDataHeaderFromRequest(size_t line_break_pos)
{
	string		 line = _buffer.substr(0, line_break_pos);
	string		 key;
	string		 value;
	const string end_of = ": ";
	const string space = " ";
	const string end_of_line = "\n";

	_headers["Method"] = getAndErase(line, space);
	_headers["Url"] = getAndErase(line, space);
	_headers["httpVersion"] = getAndErase(line, end_of_line);

	parseURI(_headers["Url"]);

	if (InvalidMethod() || InvalidHeader()) {
		return;
	}

	_buffer = _buffer.substr(line_break_pos + 2);

	while ((line_break_pos = _buffer.find("\r\n")) != string::npos) {

		line = _buffer.substr(0, line_break_pos);
		if (line.empty()) {
			break;
		}

		key = getAndErase(line, end_of);
		value = line;

		_headers[key] = value;
		_buffer = _buffer.substr(line_break_pos + 2);
	}
}

void Parser::parseRequest(const std::string &request)
{
	size_t line_break_pos = 0;

	_buffer = request;

	line_break_pos = _buffer.find("\r\n");
	getDataHeaderFromRequest(line_break_pos);

	getBodyFromRequest();

	const std::string content_type = _headers["Content-Type"];
	if (content_type.empty() ||
		content_type.find("multipart/form-data") == std::string::npos) {
		return;
	}

	_headers["body"] = request;
}

std::string Parser::findHostName(const std::string &request)
{
	const std::string host = "Host: ";
	size_t			  pos = request.find(host);

	if (pos == std::string::npos) {
		return "";
	}
	pos += host.size();
	const size_t end = request.find("\r\n", pos);
	return request.substr(pos, end - pos);
}

std::string Parser::findExtension(const std::string &filename)
{
	const size_t pos = filename.rfind('.');

	if (pos == std::string::npos) {
		return "";
	}
	return filename.substr(pos + 1);
}

std::string Parser::findFilename(const std::string &path)
{
	const size_t pos = path.rfind('/');

	if (pos == std::string::npos) {
		return "";
	}
	return path.substr(pos + 1);
}

std::string Parser::getHeader(const std::string &key) const
{
	try {
		return _headers.at(key);
	} catch (const std::out_of_range &e) {
		return "";
	}
}

Parser::~Parser() {}
