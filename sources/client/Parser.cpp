/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:58:36 by anthony           #+#    #+#             */
/*   Updated: 2024/09/24 16:06:32 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client_Parser.hpp"

client::Parser::Parser()
{
	_codeResponse = "200";
}

client::Parser::Parser(const Parser &src)
{
	if (this == &src) {
		return;
	}
	_headers = src._headers;
	_buffer = src._buffer;
	_url_path = src._url_path;
	_filename = src._filename;
	_file_extension = src._file_extension;
	_codeResponse = src._codeResponse;
}

client::Parser &client::Parser::operator=(const Parser &src)
{
	if (this == &src) {
		return *this;
	}
	_headers = src._headers;
	_buffer = src._buffer;
	_url_path = src._url_path;
	_filename = src._filename;
	_file_extension = src._file_extension;
	_codeResponse = src._codeResponse;
	return *this;
}

client::Parser::~Parser() {}

map< string, string > &client::Parser::getHeaders()
{
	return _headers;
}

const string &client::Parser::getCodeResponse()
{
	return _codeResponse;
}

const string &client::Parser::getUrlPath()
{
	return _url_path;
}

const string &client::Parser::getFilename()
{
	return _filename;
}

const string &client::Parser::getFileExtension()
{
	return _file_extension;
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
	if (_headers["Method"].empty() || _headers["httpVersion"].empty() ||
		_headers["Url"].empty() || _headers["httpVersion"] != "HTTP/1.1") {
		_codeResponse = "400";
		return true;
	}
	return false;
}

void client::Parser::getBodyFromRequest(size_t &line_break_pos)
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

void client::Parser::getHeaderFromRequest(const size_t &line_break_pos)
{
	string line = _buffer.substr(0, line_break_pos);
	string space = " ";
	string end_of_line = "\n";

	_headers["Method"] = getAndErase(line, space);
	_headers["Url"] = getAndErase(line, space);
	_headers["httpVersion"] = getAndErase(line, end_of_line);

	handleUrl(_headers["Url"]);

	if (InvalidMethod() || InvalidHeader()) {
		return;
	}

	_buffer = _buffer.substr(line_break_pos + 2);
}

void client::Parser::parseRequest(const std::string &request)
{
	size_t line_break_pos = 0;
	string line;

	_buffer = request;
	line_break_pos = _buffer.find("\r\n");
	getHeaderFromRequest(line_break_pos);

	getBodyFromRequest(line_break_pos);
}
