/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:58:36 by anthony           #+#    #+#             */
/*   Updated: 2024/10/23 11:14:25 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using client::Parser;
using std::map;
using std::string;

Parser::Parser() : _codeResponse("200") {}

bool Parser::InvalidMethod()
{
	string method = _headers["Method"];

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
	else if (_headers["httpVersion"] != "HTTP/1.1" ||
			 _headers["httpVersion"].empty()) {
		_codeResponse = "505";
		return true;
	}
	return false;
}

void Parser::getBodyFromRequest(size_t &line_break_pos)
{
	string line;
	string key;
	string value;
	string end_of = ": ";

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
	if (_buffer.empty()) {
		return;
	}
	if (!_buffer.empty()) {
		_headers["body"] = _buffer;
	}
}

void Parser::getHeaderFromRequest(const size_t &line_break_pos)
{
	string line = _buffer.substr(0, line_break_pos);
	string space = " ";
	string end_of_line = "\n";

	_headers["Method"] = getAndErase(line, space);
	_headers["Url"] = getAndErase(line, space);
	_headers["httpVersion"] = getAndErase(line, end_of_line);

	handleRequestedPath(_headers["Url"]);

	if (InvalidMethod() || InvalidHeader()) {
		return;
	}

	_buffer = _buffer.substr(line_break_pos + 2);
}

void Parser::parseRequest(const std::string &request)
{
	size_t line_break_pos = 0;
	string line;

	_buffer = request;
	line_break_pos = _buffer.find("\r\n");
	getHeaderFromRequest(line_break_pos);

	getBodyFromRequest(line_break_pos);
}

std::string Parser::findHostName(const std::string &request)
{
	std::string host = "Host: ";
	size_t		pos = request.find(host);

	if (pos == std::string::npos) {
		return "";
	}
	pos += host.size();
	size_t end = request.find("\r\n", pos);
	return request.substr(pos, end - pos);
}

std::string Parser::findExtension(const std::string &filename)
{
	size_t pos = filename.rfind('.');

	if (pos == std::string::npos) {
		return "";
	}
	return filename.substr(pos + 1);
}

std::string Parser::getHeader(const std::string &key) const
{
	try {
		return _headers.at(key);
	} catch (const std::out_of_range &e) {
		return "";
	}
};

Parser::~Parser() {}
