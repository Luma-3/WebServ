/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/09 17:01:53 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::Client(string methodType, string url, string httpVersion,
			   size_t contentLength)
{
	_headers["method"] = methodType;
	_headers["url"] = url;
	_headers["httpVersion"] = httpVersion;
	_headers["contentLength"] = std::to_string(contentLength);
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

bool Client::verifMethodType(const std::string &methodType)
{
	if (methodType == "GET" || methodType == "POST" || methodType == "DELETE" ||
		methodType.empty())
		return true;
	return false;
}

/**
 * ! Parsing
 */

void Client::parseRequest(char *params)
{
	string request(params);
	size_t pos;

	/**
	 * * Method type
	 */
	pos = request.find(' ');
	if (pos == string::npos) throw InvalidRequestException();
	_headers["method"] = request.substr(0, pos);

	if (!verifMethodType(_headers["method"])) throw InvalidRequestException();

	request.erase(0, pos + 1);

	/**
	 * * URL
	 */
	pos = request.find(' ');
	if (pos == string::npos) throw InvalidRequestException();
	_headers["url"] = request.substr(0, pos - 1);
	// TODO: check if url is valid

	request.erase(0, pos + 1);

	/**
	 * * HTTP version
	 */
	pos = request.find('\n');
	if (pos == string::npos) throw InvalidRequestException();
	_headers["httpVersion"] = request.substr(0, pos - 1);
	// TODO: check if httpVersion is valid

	request.erase(0, pos + 1);

	/**
	 * * Host
	 */
	pos = request.find("Host: ");
	if (pos == string::npos) throw InvalidRequestException();
	_headers["host"] = request.substr(pos + 6, request.find('\n') - pos - 6);
	if (_headers["host"].empty()) throw InvalidRequestException();

	request.erase(0, request.find('\n') + 1);

	if (_headers["method"] != "POST") return;

	/**
	 * * Content-Length
	 */
	pos = request.find("Content-Length: ");
	if (pos == string::npos) throw InvalidRequestException();

	request.erase(0, pos + 16);

	_headers["contentLenght"] = request.substr(0, request.find('\n'));
}
