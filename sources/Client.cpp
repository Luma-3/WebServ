/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/19 14:09:17 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include "Server.hpp"

Client::Client()
{
	_haveHeader = false;
	_codeResponse = "200";
}

Client::Client(const Server &server) : Server(server)
{
	_haveHeader = false;
	_codeResponse = "200";
}

Client::Client(const Client &src) : Server(src)
{
	if (this == &src) {
		return;
	}
	_headers = src._headers;
	_buffer = src._buffer;
	_haveHeader = src._haveHeader;
	_codeResponse = src._codeResponse;
}

Client::~Client() {}

Client &Client::operator=(const Client &src)
{
	if (this == &src) {
		return *this;
	}
	_headers = src._headers;
	_buffer = src._buffer;
	_haveHeader = src._haveHeader;
	_codeResponse = src._codeResponse;
	return *this;
}

client::Parser &Client::getParser()
{
	return _parser;
}

string Client::findStatusMessage(const std::string &code)
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

std::vector< char * > Client::readDataRequest(std::ifstream &file)
{
	file.seekg(0, std::ios::end);

	size_t size = file.tellg();

	file.seekg(0, std::ios::beg);
	std::vector< char * > body;
	body.resize(size);
	file.read(body[0], static_cast< std::streamsize >(size));
	file.close();
	return body;
}

void Client::getUrlDefaultErrorPage()
{
	_url = string(DEFAULT_ERROR_PAGE) + ToString(_codeResponse) + ".html";
}

std::vector< char * > Client::createErrorPage()
{
	std::vector< char * > body;
	std::string			  error_page = "<head><h1>ERROR " + _codeResponse +
							 findStatusMessage(_codeResponse) + "</h1></head>";
	body.assign(error_page.begin(), error_page.end());
	return body;
}

std::vector< char * > Client::getDataFromFileRequest(bool &key)
{
	while (true) {
		std::ifstream file(_url.c_str(), std::ios::binary);

		std::cout << "Url : " << _url << std::endl;
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

string Client::buildResponse()
{
	std::string response;

	response =
		"HTTP/1.1 " + _codeResponse + findStatusMessage(_codeResponse) + "\r\n";
	response += "Content-Type: " + _content_type + "\r\n";

	bool				  key = true;
	std::vector< char * > body = getDataFromFileRequest(key);

	response += "Content-Length: " + _content_length + "\r\n";
	response += "Connection: close\r\n\r\n";

	std::vector< char > response_vector(response.begin(), response.end());

	response_vector.insert(response_vector.end(), body.begin(), body.end());

	std::string final_response(response_vector.begin(), response_vector.end());

	std::cout << response << std::endl;

	return final_response;
}