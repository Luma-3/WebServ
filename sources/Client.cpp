/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/12 11:35:03 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <algorithm>
#include <fstream>

#include "Server.hpp"

Client::Client() : Server()
{
	_haveHeader = false;
	_codeResponse = "200";
}

Client::Client(string methodType, string url, string httpVersion,
			   string contentLength, string host, string contentType) :
	Server()
{
	_headers["url"] = url;
	_headers["host"] = host;
	_headers["method"] = methodType;
	_headers["httpVersion"] = httpVersion;
	_headers["contentType"] = contentType;
	_headers["contentLength"] = contentLength;
	_haveHeader = true;
	_codeResponse = "200";
}

Client::Client(char *params)
{
	try {
		parseRequest(params);
	} catch (InvalidRequestException &e) {
		std::cerr << e.what() << std::endl;
	}
}

Client::~Client() {}

Client::Client(const Server &server) : Server(server)
{
	_haveHeader = false;
	_codeResponse = "200";
}

/**
 * ! Utils
 */
void Client::checkMethod(const std::string &methodType)
{
	if (methodType != "GET" && methodType != "POST" && methodType != "DELETE")
		_codeResponse = "405";
}

void Client::checkHttp(const string &httpVersion)
{
	if (httpVersion != "HTTP/1.1") _codeResponse = "400";
}

void Client::createUrl(string &url)
{
	if (url[0] == '/' && url.length() == 1) {
		_headers["Url"] = _locations["/"]["index"];
		return;
	}

	if (url[0] != '/') url = "/" + url;

	if (url.length() > 1 && url[url.length() - 1] == '/')
		url = url.substr(0, url.length() - 1);
	/**
	 * ! Virer le slash de fin si il existe
	 */
	_headers["Url"] = _locations[url]["root"] + url + getExtensionFile();
}

void Client::checkUrl(const string &url)
{
	std::ifstream file(url.c_str());
	if (!file.good()) {
		_headers["Url"] = _locations["/404"]["root"] + "/404.html";
		_codeResponse = "404";
		_headers["Url"] = _locations["/404"]["file"];
	}
}

/**
 * ! Parsing
 */
string Client::getAndErase(string &str, const string &delim)
{
	size_t pos = str.find(delim);
	string ret = str.substr(0, pos);

	if (pos == string::npos) return ret;
	str.erase(0, pos + delim.length());
	return ret;
}

/**
 * ! Reset Headers after parsing each request
 */
void Client::resetHeaders()
{
	_headers.clear();
	_haveHeader = false;
	_codeResponse = "200";
}

/**
 * ! Parse the request
 */

string Client::getExtensionFile()
{

	string contentType = _headers["Content-Type"];
	if (contentType == "text/html") return ".html";
	if (contentType == "text/css") return ".css";
	if (contentType == "text/javascript") return ".js";
	if (contentType == "image/png") return ".png";
	if (contentType == "image/jpg") return ".jpg";
	if (contentType == "image/jpeg") return ".jpeg";
	if (contentType == "image/gif") return ".gif";
	if (contentType == "image/svg+xml") return ".svg";
	if (contentType == "image/x-icon") return ".ico";
	if (contentType == "application/json") return ".json";
	if (contentType == "application/xml") return ".xml";
	if (contentType == "application/pdf") return ".pdf";
	if (contentType == "application/zip")
		return ".zip";
	else
		return "Unknown";
}

void Client::parseRequest(void *buff)
{
	size_t pos = 0;

	_buffer += (char *)buff;
	pos = _buffer.find("\r\n");

	if (pos == string::npos) return;

	if (!_haveHeader) {

		string line = _buffer.substr(0, pos);

		// * Method type
		_headers["Method"] = getAndErase(line, " ");
		checkMethod(_headers["Method"]);

		// * Url
		_headers["Url"] = getAndErase(line, " ");
		createUrl(_headers["Url"]);
		checkUrl(_headers["Url"]);
		std::cout << "URL = " << _headers["Url"] << std::endl;

		// * Http version
		_headers["httpVersion"] = getAndErase(line, "\n");
		checkHttp(_headers["httpVersion"]);

		_buffer.erase(0, pos + 2);
		_haveHeader = true;
	}

	/**
	 * * Other headers
	 */
	while ((pos = _buffer.find("\r\n")) != string::npos) {
		string line = _buffer.substr(0, pos);

		if (line.empty()) break;

		string key = getAndErase(line, ": ");
		string value = line;
		_headers[key] = value;

		_buffer.erase(0, pos + 2);
		pos = _buffer.find("\r\n");
		_haveHeader = false;
	}
	if (_headers["Method"] != "POST") return;

	if (_headers["Content-Type"].empty()) throw InvalidRequestException();
}

std::string intToString(int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

/**
 * ! Build the response
 */
string Client::getStatusMessage(const std::string &code)
{
	if (code == "200") return " OK";
	if (code == "400") return " Bad Request";
	if (code == "401") return " Unauthorized";
	if (code == "403") return " Forbidden";
	if (code == "404") return " Not Found";
	if (code == "500") return " Internal Server Error";
	return "Unknown Status";
}

string Client::getDataFromFileRequest()
{
	string		  url = _headers["Url"];
	std::ifstream file(url.c_str());
	string		  body;
	string		  line;

	if (file.is_open()) {
		while (getline(file, line)) {
			body += line;
		}
		file.close();
	} else {
		std::cout << "PROBLEME FICHIER NON OUVERT" << std::endl;
	}
	std::cout << "Body = " << body << std::endl;
	return body;
}

string Client::buildResponse()
{
	std::string response =
		"HTTP/1.1 " + _codeResponse + getStatusMessage(_codeResponse) + "\r\n";
	response += "Content-Type: " + _headers["Content-Type"] + "\r\n";
	string body = getDataFromFileRequest();
	response += "Content-Length: " + intToString(body.length()) + "\r\n";
	response += "Connection: close\r\n\r\n";
	response += body;

	std::cout << response << std::endl;
	return response;
}