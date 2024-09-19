/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/19 14:05:28 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "Server.hpp"

using std::map;
using std::string;

#define DEFAULT_ERROR_PAGE	"default_page/html/error"
#define DEFAULT_ERROR_STYLE "default_page/css/style.css"
#define DEFAULT_ERROR_IMG	"default_page/images/bg.jpg"

namespace client {

class Client : public Server
{
  private:
	client::Parser _parser;
	string		   _url;
	string		   _content_type;
	string		   _content_length;
	string		   _codeResponse;

	std::vector< char * > readDataRequest(std::ifstream &file);
	std::vector< char * > getDataFromFileRequest(bool &key);
	void				  getUrlDefaultErrorPage();
	std::vector< char * > createErrorPage();
	static string		  findStatusMessage(const std::string &code);

  public:
	Client();
	Client(const Client &src);
	explicit Client(const Server &server);
	Client &operator=(const Client &src);
	~Client();

	client::Parser &getParser();

	string buildResponse();
};

} // namespace client

#include "Requestutils.tpp"

#endif