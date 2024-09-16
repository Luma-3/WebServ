/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/16 13:53:17 by Monsieur_Ca      ###   ########.fr       */
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
// /**
//  * ! Virer le slash de fin si il existe
//  */
// if (url_path.length() > 1 && url_path[url_path.length() - 1] == '/')
// 	url_path = url_path.substr(0, url_path.length() - 1);

string Client::getContentType(const string &file_extension)
{

	if (file_extension == "html") return "text/html";
	if (file_extension == "css") return "text/css";
	if (file_extension == "js") return "text/javascript";
	if (file_extension == "jpg") return "image/jpeg";
	if (file_extension == "jpeg") return "image/jpeg";
	if (file_extension == "png")
		return "image/png";
	else
		return "text/plain";
}

void Client::createUrl(string &url)
{
	std::cout << "URL = " << url << std::endl;

	string url_path;
	string filename;
	string file_extension;

	size_t last_slash = url.find_last_of('/');
	size_t last_dot = url.find_last_of('.');

	if (last_slash == string::npos || last_slash == 0)
		url_path = "/";
	else {
		url_path = url.substr(0, url.find_last_of('/'));
	}

	if (last_dot == string::npos) {
		file_extension = "html";
	} else {
		file_extension = url.substr(last_dot + 1);
	}

	if (last_dot == string::npos || last_dot < last_slash) {
		filename = url.substr(last_slash + 1);
	} else {
		filename = url.substr(last_slash + 1, last_dot - last_slash - 1);
	}
	std::cout << "URL_PATH = " << url_path << std::endl;
	std::cout << "FILENAME = " << filename << std::endl;
	std::cout << "FILE_EXTENSION = " << file_extension << std::endl;

	if (url_path.empty()) {
		url_path = "/";
		filename = "index";
		file_extension = "html";
	} else if (url_path[0] != '/') {
		url_path = "/" + url_path;
	}
	if (filename.empty()) filename = "index";
	if (file_extension.empty()) file_extension = "html";

	string root;
	if (_locations.find(url_path) == _locations.end()) {
		root = _locations["/"]["root"] + url_path;
	} else {
		root = _locations[url_path]["root"] + "/";
	}
	_headers["Url"] = root + filename + "." + file_extension;

	std::cout << "AU FINAL URL = " << _headers["Url"] << std::endl;
	_headers["Content-Type"] = getContentType(file_extension);
}

void Client::checkUrl(const string &url)
{
	std::cout << "URL FOR FILE TO READ = " << url << std::endl;

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

std::vector< char > Client::getDataFromFileRequest()
{
	string				url = _headers["Url"];
	std::ifstream		file(url.c_str(), std::ios::binary);
	std::vector< char > body;
	string				line;

	if (file.is_open()) {
		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		body.resize(size);
		file.read(&body[0], size);
		file.close();
	} else {
		std::cout << "PROBLEME FICHIER NON OUVERT" << std::endl;
	}
	// std::cout << "Body = " << body << std::endl;
	return body;
}

string Client::buildResponse()
{
	std::string response =
		"HTTP/1.1 " + _codeResponse + getStatusMessage(_codeResponse) + "\r\n";
	response += "Content-Type: " + _headers["Content-Type"] + "\r\n";
	std::vector< char > body = getDataFromFileRequest();
	response += "Content-Length: " + intToString(body.size()) + "\r\n";
	response += "Connection: close\r\n\r\n";

	std::vector< char > response_vector(response.begin(), response.end());

	response_vector.insert(response_vector.end(), body.begin(), body.end());

	std::string final_response(response_vector.begin(), response_vector.end());

	// std::cout << response << std::endl;
	return final_response;
}