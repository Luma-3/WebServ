/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/01 12:52:45 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

#include <iostream>

using client::Builder;
using std::map;
using std::string;

Builder::Builder() : _return_code("200") {}

Builder::Builder(const Builder &src) :
	_path(src._path),
	_url(src._url),
	_return_code(src._return_code)
{
}

Builder::~Builder() {}

Builder &Builder::operator=(const Builder &src)
{
	if (this == &src) {
		return *this;
	}
	_path = src._path;
	_url = src._url;
	_return_code = src._return_code;
	return *this;
}

void Builder::createUrlDefaultErrorPage()
{
	_url = string(DEFAULT_ERROR_PAGE) + ToString(_return_code) + ".html";
}

std::vector< char > Builder::createErrorPage()
{
	std::vector< char > body;

	std::string error_page = "<head><h1>ERROR " + _return_code +
							 findStatusMessage(_return_code) +
							 "Sorry for this ugly page bro </h1></head>";

	body.assign(error_page.begin(), error_page.end());

	return body;
}

const statement::Location *
Builder::findLocation(const std::string &requested_path)
{
	std::vector< const statement::Location * > locations;

	std::vector< const statement::Location * >::const_iterator it =
		_server->getLocations().begin();

	while (it != _server->getLocations().end()) {
		if ((*it)->getRoute() == requested_path) {
			return (*it);
		}
		++it;
	}
	return NULL;
}

// const statement::ErrorPage *
// Builder::findErrorPage(const statement::Location *location)
// {
// 	std::vector< const statement::ErrorPage * > error_page;

// 	if (location == NULL) {
// 		error_page = _server->getErrorPages();
// 	} else {
// 		error_page = location->getErrorPages();
// 	}

// 	std::vector< const statement::ErrorPage * >::const_iterator it =
// 		error_page.begin();

// 	while (it != error_page.end()) {
// 		if ((*it)->getValue() == _return_code) {
// 			return (*it);
// 		}
// 		++it;
// 	}
// 	return NULL;
// }

// void Builder::FindErrorFile(const string &requested_path)
// {
// 	const statement::Location  *location = findLocation(requested_path);
// 	const statement::ErrorPage *error_page = NULL;
// 	string						path;

// 	if (location != NULL) {
// 		error_page = findErrorPage(location);
// 		if (error_page != NULL) {
// 			if (location->getRoot().empty() == false)
// 				path = location->getRoot() + error_page->getValue();
// 			else {
// 				path = _server->getRoot() + error_page->getValue();
// 			}
// 			_url = path;
// 			return;
// 		} else {
// 			error_page = findErrorPage();
// 			if (error_page != NULL) {
// 				path = _server->getRoot() + error_page->getValue();
// 			} else {
// 				path = NULL;
// 			}
// 		}
// 	}
// }

std::vector< char > Builder::readDataRequest(std::ifstream &file)
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

std::vector< char > Builder::getDataFromFileRequest(bool &key)
{
	while (true) {
		std::ifstream file(_url.c_str(), std::ios::binary);

		// if (_parser.getFileExtension() == "py") {
		// 	return handleCGI();
		// }
		if (file.is_open()) {
			return readDataRequest(file);
		}
		if (key) {
			createUrlDefaultErrorPage();
			key = false;
		} else {
			break;
		}
	}
	return createErrorPage();
}

void Builder::findFinalFileFromUrl(const std::string &url_path,
								   const Parser		 &parser)
{
	if (_return_code != "200") {
		// FindErrorFile(url_path);

		return;
	}

	std::vector< const statement::Location * >::const_iterator it =
		_server->getLocations().begin();

	std::cout << "SIZE LOCATIONS : " << _server->getLocations().size()
			  << std::endl;

	while (it != _server->getLocations().end()) {
		std::cout << "URL PATH : " << url_path << std::endl;
		std::cout << "ROUTE : " << (*it)->getRoute() << std::endl;
		if ((*it)->getRoute() == url_path) {
			_path = (*it)->getRoot();
			std::cout << "PATH : " << _path << std::endl;
			break;
		}
		++it;
	}
	_url = _path + parser.getFilename() + "." + parser.getFileExtension();
	std::cout << "URL BEFORE ACCESS : " << _url << std::endl;

	if (access(_url.c_str(), F_OK) != 0) {
		if (errno == ENOENT) {
			_return_code = "404";
		} else {
			_return_code = "402";
		}
		// FindErrorFile(url_path);
	}
}

string Builder::BuildResponse(client::Parser &parser, const Server *server,
							  const Server *default_server)
{
	_server = server;
	(void)default_server;

	map< string, string > _headers = parser.getHeaders();
	bool				  key = true;
	_return_code = parser.getCodeResponse();

	findFinalFileFromUrl(parser.getRequestedPath(), parser);

	std::vector< char > body = getDataFromFileRequest(key);

	string response =
		"HTTP/1.1 " + _return_code + findStatusMessage(_return_code) + "\r\n";
	response +=
		"Content-Type: " + findContentType(parser.getFileExtension()) + "\r\n";

	response += "Content-Length: " + ToString(body.size()) + "\r\n";

	if (_headers["Connection"] == "close") {
		response += "Connection: close\r\n\r\n";
	} else {
		response += "Connection: keep-alive\r\n\r\n";
	}
	response += std::string(body.begin(), body.end());

	// std::cout << "Response : " << response << std::endl;
	reset();
	return response;
}

void Builder::reset()
{
	_server = NULL;
	_path.clear();
	_url.clear();
	_return_code = "200";
}