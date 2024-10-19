/* **************************************************************************
 */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:54:01 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/18 12:38:42 by jbrousse         ###   ########.fr       */
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

#include "client/Client.hpp"
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
	const VirtualServer	 *_server;
	const VirtualServer	 *_default_server;
	const client::Parser &_parser;

	std::string			_request_path;
	std::string			_path;
	std::string			_filename;
	std::string			_extension;
	std::string			_location;
	std::string			_code;
	std::vector< char > _body;

	void createErrorPage();

	void		buildHeader();
	int			readDataRequest();
	static void trimPath(std::string &path);

	int	 verifLocationAndGetNewPath();
	bool verifAccess();
	void insertFileInHead(std::string &file);
	void insertFooterAndSetAttributes(std::vector< char > &body);

  public:
	Builder(const VirtualServer *server, const VirtualServer *default_server,
			const client::Parser &parser);
	~Builder();

	void BuildResponse(std::string &response);
	void returnParam();
	void findFile();
	void readFile();
	void findErrorpageLocationServer();
	void setIndexOrReturnAutoindex();

	const std::string &getFileExtension() const { return _extension; };
	const std::string &getFilename() const { return _filename; };
	const std::string &getCode() const { return _code; };
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
