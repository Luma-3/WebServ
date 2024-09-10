/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/10 17:36:22 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#include <algorithm>
#include <fstream>

Client::Client() : Server() {}

Client::Client(string methodType, string url, string httpVersion,
			   string contentLength, string host, string contentType) : Server()
{
	_headers["url"] = url;
	_headers["host"] = host;
	_headers["method"] = methodType;
	_headers["httpVersion"] = httpVersion;
	_headers["contentType"] = contentType;
	_headers["contentLength"] = contentLength;
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

/**
 * ! Utils
 */
void	Client::checkMethod(const std::string &methodType)
{
	if (methodType != "GET"
	&& methodType != "POST"
	&& methodType != "DELETE")
		throw InvalidRequestException();
}

void	Client::checkHttp(const std::string &httpVersion)
{
	if (httpVersion != "HTTP/1.1") throw InvalidRequestException();
}

void	Client::createUrl(std::string &url)
{
	if (url[0] ==  '/' && url.length() == 1)
		return;

	if (url[0] != '/')
		url = "/" + url;
	
	if (url.length() > 1 && url[url.length() - 1] == '/')
		url = url.substr(0, url.length() - 1);
	/**
	 * ! Virer le slash de fin si il existe
	 */
	_headers["Url"] = _locations[url]["root"] + url;
}

void	Client::checkUrl(const std::string &url)
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
	if (pos == string::npos) return ret;
	str.erase(0, pos + delim.length());
	return ret;
}

void	Client::parseRequest(char *params)
{

	string request(params);

	/**
	 * * Method type
	 */
	_headers["Method"] = getAndErase(request, " ");
	checkMethod(_headers["Method"]);

	/**
	 * * URL
	 */
	_headers["Url"] = getAndErase(request, " ");
	createUrl(_headers["Url"]);
	// checkUrl(_headers["Url"]);

	/**
	 * * HTTP version
	 */
	_headers["httpVersion"] = getAndErase(request, "\n");
	checkHttp(_headers["httpVersion"]);

	/**
	 * * Headers
	 */
	size_t pos;
	while ((pos = request.find("\n")) != string::npos)
	{
		string line = request.substr(0, pos);
		request.erase(0, pos + 1);

		if (line.empty()) break;

		string key = getAndErase(line, ": ");
		string value = line;
		_headers[key] = value;
	}
	if (_headers["Method"] != "POST") return ;

	if (_headers["Content-Type"].empty()) throw InvalidRequestException();
}

// void	Client::