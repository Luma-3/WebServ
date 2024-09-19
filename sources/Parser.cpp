/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:58:36 by anthony           #+#    #+#             */
/*   Updated: 2024/09/19 22:20:51 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

client::Parser::Parser()
{
	_haveHeader = false;
	_codeResponse = "200";
}

client::Parser::~Parser() {}

map< string, string > &client::Parser::getHeaders()
{
	return _headers;
}

string &client::Parser::getCodeResponse()
{
	return _codeResponse;
}

string &client::Parser::getUrlPath()
{
	return _url_path;
}

string &client::Parser::getFilename()
{
	return _filename;
}

string &client::Parser::getFileExtension()
{
	return _file_extension;
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
		_headers["httpVersion"].empty() ||
		_headers["httpVersion"] != "HTTP/1.1") {
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

void client::Parser::getHeaderFromRequest(const size_t &line_break_pos)
{
	string line = _buffer.substr(0, line_break_pos);

	std::cout << "Header : " << line << std::endl;
	_headers["Method"] = getAndErase(line, " ");
	_headers["Url"] = getAndErase(line, " ");
	_headers["httpVersion"] = getAndErase(line, "\n");

	handleUrl(_headers["Url"]);

	if (InvalidMethod() || InvalidHeader()) {
		return;
	}
	_buffer.erase(0, line_break_pos + 2);
	_haveHeader = true;
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
}
