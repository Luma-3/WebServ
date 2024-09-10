/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/10 16:49:36 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <map>

#include "Server.hpp"

using std::string;
using std::map;

class Client : public Server
{
  private:
	void	checkMethod(const string &methodType);
	void	checkHttp(const string &httpVersion);
	void	checkUrl(const string &url);
	void	createUrl(std::string &url);
	string getAndErase(string &str, const string &delim);



  public:
	map<string, string> _headers;

	Client();
	Client(char *params);
	Client(string methodType, string url, string httpVersion,
		   string contentLength, string host, string contentType);
	~Client();

	void	parseRequest(char *params);
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