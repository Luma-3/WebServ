/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/18 11:32:00 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <cstring>
#include <dirent.h>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <vector>

#include "client/Parser.hpp"
#include "server/VirtualServer.hpp"

#define CHILD_BUFFER_SIZE 4096

#define INDEX_FIND	  0
#define AUTOINDEX_OFF 1
#define AUTOINDEX_ON  2
#define NONE		  -1

namespace client {

class Client;

class Builder
{
  private:
	const VirtualServer *_server;
	const VirtualServer *_default_server;

	std::string _path;
	std::string _final_url;
	std::string _code;
	std::string _response;

	// SUR
	std::string _content_type;

	void createErrorPage(const std::string	 &return_code,
						 std::vector< char > &body);

	void buildHeader(const Parser &parser, const std::string &location_param,
					 int body_size);
	static std::string findContentType(const std::string &file_extension);
	void			   reset();

	void findBodyErrorPage(const Parser &parser, std::vector< char > &body);
	int	 readDataRequest(std::vector< char > &body, const std::string &path);
	int	 findErrorpageLocationServer(const VirtualServer *server,
									 const std::string	 &code,
									 std::vector< char > &body,
									 const std::string	 &path);
	void findFile(const client::Parser &parser, std::vector< char > &body);
	void readFile(const client::Parser &parser, const std::string &path,
				  std::vector< char > &body);
	bool returnParam(Parser &parser);
	void indexOrAutoindexList(const client::Parser &parser,
							  std::vector< char >  &body);
	bool isDirRequest(const std::string &path);
	void trimPath(std::string &path);

	int	 verifLocationAndGetNewPath(const client::Parser &parser,
									std::string			 &final_path,
									std::vector< char >	 &body);
	bool verifAccess(const client::Parser &parser, const std::string &new_path,
					 std::vector< char > &body);
	void insertFileInHead(std::string &file, std::string &head,
						  std::vector< char > &body);
	void insertFooterAndSetAttributes(std::vector< char > &body);
	void findRootDirectory(const std::string &path, std::string &root);

  public:
	Builder(const VirtualServer *server, const VirtualServer *default_server);
	Builder(const Builder &src);
	Builder &operator=(const Builder &src);
	~Builder();

	void BuildResponse(client::Parser &parser);

	const std::string &getResponse() const;
};

} // namespace client

std::string findStatusMessage(const std::string &code); // namespace client

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

#define DEFAULT_AUTOINDEX_PAGE_HEAD \
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
	<title>Autoindex</title>\
</head>\
<body>"

#define DEFAULT_AUTOINDEX_RETURN_BUTTON                     \
	"<div class=\"container\"><div class=\"title\"><li><a " \
	"href=\"%@return_path@%\" style=\"color: "              \
	"black;\">\"Back\"</a></li></div></div>"

#define DEFAULT_AUTOINDEX_LIST                                                \
	"<div class=\"container\"><div class=\"title\"><li><a href=\"%@file@%\" " \
	"style=\"color: "                                                         \
	"black;\">%@file@%</a></li></div></div>"

#define DEFAULT_AUTOINDEX_PAGE_FOOT "</body></html>"

#endif
