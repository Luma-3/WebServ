/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:15:36 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/25 00:59:34 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

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
	_path = src._path;
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
	_path = src._path;
	_url = src._url;
	_return_code = src._return_code;
	return *this;
}

client::Parser &client::Client::getParser()
{
	return _parser;
}

void client::Client::createUrlDefaultErrorPage()
{
	_url = string(DEFAULT_ERROR_PAGE) + ToString(_return_code) + ".html";
}

std::vector< char > client::Client::createErrorPage()
{
	std::vector< char > body;

	std::string error_page = "<head><h1>ERROR " + _return_code +
							 findStatusMessage(_return_code) +
							 "Sorry for this ugly page bro < / h1 >< / head > ";

	body.assign(error_page.begin(), error_page.end());

	return body;
}

void client::Client::findErrorFile(string &url_path)
{
	(void)url_path;

	_url = DEFAULT_ERROR_PAGE + ToString(_return_code) + ".html";
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

// #include <iostream>
// std::vector< char > client::Client::handleCGI()
// {
// 	std::cout << "GO TO CGI" << std::endl;
// 	int	  pipefd[2];
// 	pid_t pid;

// 	pipe(pipefd);
// 	// TODO : check if pipe is correctly created

// 	pid = fork();
// 	// TODO : check if fork is correctly created

// 	if (pid == 0) {
// 		const char *interpreter = "usr/bin/python3";
// 		const char *script_path = _url.c_str();

// 		close(pipefd[0]);
// 		dup2(pipefd[1], 1);
// 		close(pipefd[1]);

// 		const char *argv[] = {interpreter, script_path, NULL};
// 		const char *envp[] = {"PATH=/usr/bin", NULL};

// 		std::cout << "EXECUTE CGI" << std::endl;
// 		std::cout << "interpreter : " << interpreter << std::endl;
// 		std::cout << "script_path : " << script_path << std::endl;
// 		execve(interpreter, (char *const *)argv, (char *const *)envp);
// 		_exit(1);
// 		// TODO : check if execlp is correctly executed
// 	} else {
// 		close(pipefd[1]);
// 		std::vector< char > body;
// 		char				buffer[CHILD_BUFFER_SIZE];
// 		int					bytes_read;

// 		while ((bytes_read = read(pipefd[0], buffer, CHILD_BUFFER_SIZE)) > 0) {
// 			body.insert(body.end(), buffer, buffer + bytes_read);
// 		}
// 		close(pipefd[0]);
// 		waitpid(pid, NULL, 0);
// 		return body;
// 	}
// }

std::vector< char > client::Client::getDataFromFileRequest(bool &key)
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

#include <iostream>
void client::Client::findFinalFileFromUrl()
{
	string url_path = _parser.getUrlPath();

	if (_return_code != "200") {
		return findErrorFile(url_path);
	}

	std::vector< const statement::Location * >::const_iterator it =
		getLocations().begin();

	while (it != getLocations().end()) {
		std::cout << "LOCATION : " << (*it)->getRoute() << std::endl;
		if ((*it)->getRoute() == url_path) {
			_path = (*it)->getRoot();
			break;
		}
		(it++);
	}
	if (!_path.empty()) {
		_path = _path.substr(1, _path.size() - 1);
	}
	// else {
	// 	_path = DEFAULT_ERROR_PAGE
	// }
	_url = _path + _parser.getFilename() + "." + _parser.getFileExtension();
	std::cout << "URL BEFORE ACCESS : " << _url << std::endl;

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

	std::cout << "GO to buildResponse" << std::endl;
	_return_code = _parser.getCodeResponse();

	std::cout << "GO to findFinalFileFromUrl" << std::endl;
	findFinalFileFromUrl();

	std::cout << "GO to getDataFromFileRequest" << std::endl;
	std::vector< char > body = getDataFromFileRequest(key);

	std::cout << "GO to response" << std::endl;
	string response =
		"HTTP/1.1 " + _return_code + findStatusMessage(_return_code) + "\r\n";
	response +=
		"Content-Type: " + findContentType(_parser.getFileExtension()) + "\r\n";

	response += "Content-Length: " + ToString(body.size()) + "\r\n";

	if (_headers["Connection"] == "close") {
		response += "Connection: close\r\n\r\n";
	} else {
		response += "Connection: keep-alive\r\n\r\n";
	}
	response += std::string(body.begin(), body.end());

	return response;
}
