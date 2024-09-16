/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/16 19:14:03 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
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

string Client::getStatusMessage(const std::string &code)
{
	if (code == "200") return " OK";
	if (code == "400") return " Bad Request";
	if (code == "403") return " Forbidden";
	if (code == "404") return " Not Found";
	if (code == "405") return " Method Not Allowed";
	if (code == "500") return " Internal Server Error";
	return "Unknown Status";
}

template <typename T>
std::string ToString(const T &value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::vector< char > Client::getDataFromFileRequest(bool &key)
{
	string				url;
	string				error_page;
	std::vector< char > body;

	url = _headers["Url"];
	
	std::ifstream file(url.c_str(), std::ios::binary);

	std::cout << "Url : " << url << std::endl;
	if (file.is_open()) {
		file.seekg(0, std::ios::end);
	
		size_t size = file.tellg();

		file.seekg(0, std::ios::beg);
		body.resize(size);
		file.read(&body[0], size);
		file.close();
		std::cout << "On return le body" << std::endl;
		return body;
	}
	if (key == true) {
		key = false;
		error_page = string(DEFAULT_ERROR_PAGE) + ToString(_codeResponse) + ".html";
		_headers["Url"] = error_page;
		std::cout << "error page URL : " << error_page << std::endl;
		return getDataFromFileRequest(key);
	}
	error_page = "<head><h1>ERROR " + _codeResponse + getStatusMessage(_codeResponse) + "</h1></head>";
	body.assign(error_page.begin(), error_page.end());

	return body;
}



string Client::buildResponse()
{
	std::string response;
	
	response = "HTTP/1.1 " + _codeResponse + getStatusMessage(_codeResponse) + "\r\n";
	response += "Content-Type: " + _headers["Content-Type"] + "\r\n";

	bool key = true;
	std::vector< char > body = getDataFromFileRequest(key);
	
	response += "Content-Length: " + ToString(body.size()) + "\r\n";
	response += "Connection: close\r\n\r\n";

	std::vector< char > response_vector(response.begin(), response.end());

	response_vector.insert(response_vector.end(), body.begin(), body.end());

	std::string final_response(response_vector.begin(), response_vector.end());

	std::cout << response << std::endl;
	return final_response;
}