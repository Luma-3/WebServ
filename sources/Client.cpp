/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/11 15:05:34 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <algorithm>
#include <fstream>

#include "Server.hpp"

Client::Client() : Server()
{
	_haveHeader = false;
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

Client::Client(const Server &server) : Server(server) {}

/**
 * ! Utils
 */
void Client::checkMethod(const std::string &methodType)
{
	if (methodType != "GET" && methodType != "POST" && methodType != "DELETE")
		throw InvalidRequestException();
}

void Client::checkHttp(const std::string &httpVersion)
{
	if (httpVersion != "HTTP/1.1") throw InvalidRequestException();
}

void Client::createUrl(std::string &url)
{
	if (url[0] == '/' && url.length() == 1) {
		_headers["Url"] = "/var/www/html/index.html";
		return;
	}

	if (url[0] != '/') url = "/" + url;

	if (url.length() > 1 && url[url.length() - 1] == '/')
		url = url.substr(0, url.length() - 1);
	/**
	 * ! Virer le slash de fin si il existe
	 */
	_headers["Url"] = _locations[url]["root"] + url;
}

void Client::checkUrl(const std::string &url)
{
	std::ifstream file(url.c_str());
	if (!file.good())
		_headers["Url"] = _locations["/404"]["root"] + "/404.html";
}

/**
 * ! Parsing
 */
string Client::getAndErase(string &str, const string &delim)
{
	size_t pos = str.find(delim);
	string ret = str.substr(0, pos);

	std::cout << "extrated : " << ret << std::endl;

	if (pos == string::npos) return ret;
	str.erase(0, pos + delim.length());
	std::cout << "After erase : " << str << std::endl;
	return ret;
}

void Client::parseRequest(void *buff)
{
	size_t pos = 0;

	_buffer += (char *)buff;
	pos = _buffer.find("\r\n");

	if (pos == string::npos) return;

	if (_haveHeader == false) {
		string line = _buffer.substr(0, pos);
		// std::cout << line << std::endl;
		// * Method type
		_headers["Method"] = getAndErase(line, " ");
		checkMethod(_headers["Method"]);

		std::cout << line << std::endl;
		// * Url
		_headers["Url"] = getAndErase(line, " ");
		createUrl(_headers["Url"]);
		// checkUrl(_headers["Url"]);

		// std::cout << line << std::endl;
		// * Http version
		_headers["httpVersion"] = getAndErase(line, "\n");
		// checkHttp(_headers["httpVersion"]);

		_buffer.erase(0, pos + 2);
		_haveHeader = true;
	}
	/**
	 * * Other headers
	 */
	pos = _buffer.find("\r\n");
	while (pos != string::npos) {
		string line = _buffer.substr(0, pos);
		std::cout << "line : " << line << std::endl;

		if (line.empty()) break;

		string key = getAndErase(line, ": ");
		string value = line;
		_headers[key] = value;
		std::cout << "key : " << key << " value : " << value << std::endl;
		std::cout << "value : " << value << std::endl;

		_buffer.erase(0, pos + 2);
		pos = _buffer.find("\r\n");
	}
	if (_headers["Method"] != "POST") return;

	if (_headers["Content-Type"].empty()) throw InvalidRequestException();
}