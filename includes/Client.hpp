/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/16 19:04:52 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unistd.h>


#include "Server.hpp"

using std::map;
using std::string;

# define DEFAULT_ERROR_PAGE "default_page/html/error"
# define DEFAULT_ERROR_STYLE "default_page/css/style.css"
# define DEFAULT_ERROR_IMG "default_page/images/bg.jpg"



class Client : public Server
{
  private:
	void				createUrl(std::string &url);
	string				getAndErase(string &str, const string &delim);
	string				getStatusMessage(const std::string &code);
	string				findContentType(const std::string &file_extension);
	std::vector< char > getDataFromFileRequest(bool &key);
	void				getHeaderFromRequest(const size_t &line_break_pos);
	void 				getBodyFromRequest(size_t &line_break_pos);
	bool				checkRequest();
	void				changeUrlError();

  public:
	map< string, string > _headers;
	string				  _buffer;
	bool				  _haveHeader;
	string				  _codeResponse;

	Client();
	Client(const Server &server);
	Client(char *params);
	Client(string methodType, string url, string httpVersion,
		   string contentLength, string host, string contentType);
	~Client();

	string buildResponse();
	void   parseRequest(void *buff);
	void   resetHeaders();
};

class InvalidRequestException : public std::exception
{
  public:
	virtual const char *what() const throw()
	{
		return "Invalid request";
	}
};

#endif