/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/09 16:38:05 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <map>

using std::string;

class Client
{
  private:
	std::map<string, string> _headers;

	void parseRequest(char *params);
	bool verifMethodType(const std::string &methodType);

  public:
	Client();
	Client(string methodType, string url, string httpVersion,
		   size_t contentLength);
	Client(char *params);
	~Client();
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