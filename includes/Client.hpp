/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/11 14:14:57 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <map>
#include <vector>

#include "Server.hpp"

using std::map;
using std::string;

class Client : public Server
{
  private:
	void   checkMethod(const string &methodType);
	void   checkHttp(const string &httpVersion);
	void   checkUrl(const string &url);
	void   createUrl(std::string &url);
	string getAndErase(string &str, const string &delim);

  public:
	map< string, string > _headers;
	string				  _buffer;
	bool				  _haveHeader;

	Client();
	Client(const Server &server);
	Client(char *params);
	Client(string methodType, string url, string httpVersion,
		   string contentLength, string host, string contentType);
	~Client();

	void parseRequest(void *buff);
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