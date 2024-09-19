/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/19 22:10:32 by anthony          ###   ########.fr       */
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

#include "Parser.hpp"
#include "Server.hpp"

using std::map;
using std::string;

#define DEFAULT_ERROR_PAGE "default_page/html/error"

namespace client {

class Client : public Server
{
  private:
	client::Parser _parser;
	string		   _url;
	string		   _return_code;

	std::vector< char > readDataRequest(std::ifstream &file);
	std::vector< char > getDataFromFileRequest(bool &key);
	std::vector< char > createErrorPage();
	void				getUrlDefaultErrorPage();
	static string		findStatusMessage(const std::string &code);
	void				findFinalFileFromUrl();
	static string		findContentType(const std::string &file_extension);
	void				findErrorFile(string &url_path);

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