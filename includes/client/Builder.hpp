/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/10 15:41:31 by Monsieur_Ca      ###   ########.fr       */
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

#define DEFAULT_ERROR_PAGE "default_page/html/"
#define CHILD_BUFFER_SIZE  4096

namespace client {

class Builder
{
  private:
	const Server *_server;
	std::string	  _path;

	std::string _final_url;
	std::string _response;

	std::vector< char > readDataRequest();
	std::vector< char > createErrorPage(const std::string &return_code);

	void accessRequestedFile(Parser &parser);
	void findDefaultErrorPath(Parser &parser);
	void findErrorPath(Parser &parser);

	void buildHeader(const Parser &parser, std::vector< char > &body);
	static std::string findContentType(const std::string &file_extension);
	void			   reset();

  public:
	Builder();
	Builder(const Builder &src);
	Builder &operator=(const Builder &src);
	~Builder();

	void BuildResponse(client::Parser &parser, const Server *server,
					   const Server *default_server);

	const std::string &getResponse() const;
};

} // namespace client

std::string findStatusMessage(const std::string &code); // namespace client

#endif