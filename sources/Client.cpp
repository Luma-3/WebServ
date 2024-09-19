/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/19 22:19:11 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

client::Client::Client()
{
	_return_code = "200";
}

client::Client::Client(const Server &server) : Server(server) {}

client::Client::Client(const Client &src) : Server(src)
{
	if (this == &src) {
		return;
	}
	_parser = src._parser;
	_url = src._url;
	_return_code = src._return_code;
}

client::Client::~Client() {}

client::Client &client::Client::operator=(const Client &src)
{
	if (this == &src) {
		return *this;
	}
	_parser = src._parser;
	_url = src._url;
	_return_code = src._return_code;
	return *this;
}

client::Parser &client::Client::getParser()
{
	return _parser;
}

string client::Client::findContentType(const string &file_extension)
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

string client::Client::findStatusMessage(const std::string &code)
{
	static map< std::string, std::string > status_message;
	if (status_message.empty()) {
		status_message["200"] = " OK";
		status_message["400"] = " Bad Request";
		status_message["403"] = " Forbidden";
		status_message["404"] = " Not Found";
		status_message["405"] = " Method Not Allowed";
		status_message["500"] = " Internal Server Error";
	}

	map< string, string >::const_iterator it = status_message.find(code);
	if (it != status_message.end()) {
		return it->second;
	}
	return "Unknown Status";
}

void client::Client::getUrlDefaultErrorPage()
{
	_url = string(DEFAULT_ERROR_PAGE) + ToString(_return_code) + ".html";
}

std::vector< char > client::Client::createErrorPage()
{
	std::vector< char > body;
	std::string			error_page = "<head><h1>ERROR " + _return_code +
							 findStatusMessage(_return_code) + "</h1></head>";
	body.assign(error_page.begin(), error_page.end());
	return body;
}

std::vector< char > client::Client::readDataRequest(std::ifstream &file)
{
	file.seekg(0, std::ios::end);

	size_t size = file.tellg();

	file.seekg(0, std::ios::beg);
	std::vector< char > body;
	body.resize(size);
	file.read(&body[0], static_cast< std::streamsize >(size));
	file.close();
	return body;
}

std::vector< char > client::Client::getDataFromFileRequest(bool &key)
{
	while (true) {
		std::ifstream file(_url.c_str(), std::ios::binary);

		if (file.is_open()) {
			return readDataRequest(file);
		}
		if (key) {
			getUrlDefaultErrorPage();
			key = false;
		} else {
			break;
		}
	}
	return createErrorPage();
}

void client::Client::findErrorFile(string &url_path)
{

	if (_locations[url_path]["root"].empty()) {
		_url = DEFAULT_ERROR_PAGE + ToString(_return_code) + ".html";
	} else {
		_url = _locations[_return_code]["error_page"];
	}
}

void client::Client::findFinalFileFromUrl()
{
	string url_path = _parser.getUrlPath();

	if (_return_code != "200") {
		return findErrorFile(url_path);
	}

	string path;

	if (_locations[url_path]["root"].empty()) {
		path = _locations["/"]["root"] + url_path;
	} else {
		path = _locations[url_path]["root"] + "/";
	}
	_url = path + _parser.getFilename() + "." + _parser.getFileExtension();

	if (access(_url.c_str(), F_OK) != 0) {
		if (errno == ENOENT) {
			_return_code = "404";
		} else {
			_return_code = "402";
		}
		findErrorFile(url_path);
	}
}

string client::Client::buildResponse()
{
	map< string, string > _headers = _parser.getHeaders();
	bool				  key = true;

	_return_code = _parser.getCodeResponse();
	findFinalFileFromUrl();

	std::vector< char > body = getDataFromFileRequest(key);

	string response =
		"HTTP/1.1 " + _return_code + findStatusMessage(_return_code) + "\r\n";
	response +=
		"Content-Type: " + findContentType(_parser.getFileExtension()) + "\r\n";

	response += "Content-Length: " + ToString(body.size()) + "\r\n";

	response += "Connection: close\r\n\r\n";

	response += std::string(body.begin(), body.end());

	return response;
}
