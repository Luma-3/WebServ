/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/25 15:07:37 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <fstream>
#include <string>
#include <sys/wait.h>
#include <vector>

#include "client/Parser.hpp"
#include "server/Server.hpp"

using std::map;
using std::string;

#define DEFAULT_ERROR_PAGE "default_page/html/error"
#define CHILD_BUFFER_SIZE  4096

namespace client {

class Client : public Server
{
  private:
	client::Parser _parser;
	string		   _path;
	string		   _url;
	string		   _return_code;

	static std::vector< char > readDataRequest(std::ifstream &file);
	std::vector< char >		   getDataFromFileRequest(bool &key);

	std::vector< char > createErrorPage();
	void				createUrlDefaultErrorPage();

	static string findStatusMessage(const std::string &code);
	static string findContentType(const std::string &file_extension);
	void		  findErrorFile(string &url_path);
	void		  findFinalFileFromUrl();

	// std::vector< char > handleCGI();

  public:
	Client();
	Client(const Client &src);
	explicit Client(const Server &server);
	Client &operator=(const Client &src);
	~Client();

	client::Parser &getParser();
	void			setParser(const Parser &parser);

	string buildResponse();
};

} // namespace client

#endif