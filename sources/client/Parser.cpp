/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:58:36 by anthony           #+#    #+#             */
/*   Updated: 2024/10/15 12:33:58 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using client::Parser;
using std::map;
using std::string;

Parser::Parser()
{
	_codeResponse = "200";
}

Parser::Parser(const Parser &src)
{
	if (this == &src) {
		return;
	}
	_headers = src._headers;
	_buffer = src._buffer;
	_path = src._path;
	_filename = src._filename;
	_codeResponse = src._codeResponse;
}

Parser::Parser(const VirtualServer *server,
			   const VirtualServer *default_server) :
	_server(server),
	_default_server(default_server),
	_codeResponse("200")
{
}

Parser &Parser::operator=(const Parser &src)
{
	if (this == &src) {
		return *this;
	}
	_headers = src._headers;
	_buffer = src._buffer;
	_path = src._path;
	_filename = src._filename;
	_codeResponse = src._codeResponse;
	return *this;
}

const map< string, string > &Parser::getHeaders() const
{
	return _headers;
}

const string &Parser::getCodeResponse() const
{
	return _codeResponse;
}

const string &Parser::getRequestedPath() const
{
	return _requested_path;
}

const string &Parser::getPath() const
{
	return _path;
}

const string &Parser::getFilename() const
{
	return _filename;
}

void Parser::setPath(const string &path)
{
	_path = path;
}

void Parser::setCodeResponse(const string &code)
{
	_codeResponse = code;
}

void Parser::setFilename(const string &url)
{
	_filename = url.substr(url.find_last_of('/') + 1);
}

void Parser::setPathAndFilename(const string &path, const string &filename)
{
	_path = path;
	_filename = filename;
}

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
	if (_headers["Method"].empty() || _headers["httpVersion"].empty() ||
		_headers["Url"].empty() || _headers["httpVersion"] != "HTTP/1.1") {
		_codeResponse = "400";
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
	std::cout << "Request : " << request << std::endl;
	line_break_pos = _buffer.find("\r\n");
	getHeaderFromRequest(line_break_pos);

	getBodyFromRequest(line_break_pos);
}

void Parser::reset()
{
	_headers.clear();
	_buffer.clear();
	_path.clear();
	_filename.clear();
	_codeResponse = "200";
}

Parser::~Parser() {}

// NEW

std::string findHostName(const std::string &request)
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
