/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/16 11:00:42 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <fstream>
#include <string>
#include <sys/wait.h>
#include <vector>

#include "client/Client.hpp"
#include "client/Parser.hpp"
#include "server/VirtualServer.hpp"

#define CHILD_BUFFER_SIZE 4096

namespace client {

class Client;

class Builder
{
  private:
	const VirtualServer	 *_server;
	const VirtualServer	 *_default_server;
	const client::Client *_client;

	std::string _path;
	std::string _final_url;
	std::string _code;
	std::string _response;

	// SUR
	std::string _content_type;

	void createErrorPage(const std::string	 &return_code,
						 std::vector< char > &body);

	void accessRequestedFile(Parser &parser);
	void findErrorPath(Parser &parser);
	void buildHeader(const Parser &parser, const std::string &location_param,
					 int body_size);

	void reset();

	void findBodyErrorPage(const Parser &parser, std::vector< char > &body);
	int	 readDataRequest(std::vector< char > &body, const std::string &path);
	int	 findErrorpageLocationServer(const VirtualServer *server,
									 const std::string	 &code,
									 std::vector< char > &body,
									 const std::string	 &path);
	void findFile(const client::Parser &parser, std::vector< char > &body);

	void findIndex(const client::Parser &parser, std::vector< char > &body);
	void readFile(const client::Parser &parser, const std::string &path,
				  std::vector< char > &body);
	bool returnParam(Parser &parser);
	void buildReturnResponse(const client::Parser, std::vector< char > &body);

  public:
	Builder(const VirtualServer *server, const VirtualServer *default_server,
			const client::Client *client);
	Builder(const Builder &src);
	Builder &operator=(const Builder &src);
	~Builder();

	void BuildResponse(client::Parser &parser);

	const std::string &getResponse() const;
};

} // namespace client

std::string findStatusMessage(const std::string &code);
std::string findContentType(const std::string &file_extension);

#define DEFAULT_ERROR_PAGE \
	"<!DOCTYPE html> \
<html lang=\"en\">\
<head>\
	<style>\
	.container {width: 80%;margin: auto;overflow: hidden;}\
	.title {color: white;font-size: 2rem;text-align: center;justify-content: center;border: 4px solid black;padding: 1rem;border-radius: 10px;background-color: #EE92C2;margin-bottom: 20px;margin-top: 20px;}\
	.errorMessage {width: 100%;height: 30%;padding: 0;margin: 0 auto 0 auto;position: center;display: flex;justify-content: center;background-color: #EE92C2;align-items: center;flex-wrap: wrap;}\
	</style>\
	<meta charset=\"UTF-8\">\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	<title>%@title@%</title>\
</head>\
<body>\
	<div class=\"container\"><div class=\"title\"><h2>%@title@%</h2><br></div></div>\
	<div class=\"errorMessage\"><h3>%@message@%</h3></div>\
</body>\
</html>"

#endif
