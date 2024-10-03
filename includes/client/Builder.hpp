/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/03 15:50:39 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <fstream>
#include <string>
#include <sys/wait.h>
#include <vector>

#include "client/Parser.hpp"
#include "server/Server.hpp"

#define DEFAULT_ERROR_PAGE "default_page/html/error"
#define CHILD_BUFFER_SIZE  4096

namespace client {

class Builder
{
  private:
	const Server *_server;
	std::string	  _path;

	std::string _url;
	std::string _return_code;

	static std::vector< char > readDataRequest(std::ifstream &file);
	std::vector< char >		   getDataFromFileRequest(bool &key);

	std::vector< char > createErrorPage();
	void				createUrlDefaultErrorPage();

	// static std::string findStatusMessage(const std::string &code);
	static std::string findContentType(const std::string &file_extension);
	void			   FindErrorFile(const std::string &requires_path);
	void			   findFinalFileFromUrl(const std::string &url_path,
											const Parser	  &parser);

	const statement::Location *findLocation(const std::string &requested_path);
	const statement::ErrorPage		*
	 findErrorPage(const statement::Location *location = NULL);
	void reset();

  public:
	Builder();
	Builder(const Builder &src);
	Builder &operator=(const Builder &src);
	~Builder();

	std::string BuildResponse(client::Parser &parser, const Server *server,
							  const Server *default_server);
};

} // namespace client

std::string findStatusMessage(const std::string &code); // namespace client

#endif