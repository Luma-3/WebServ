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
#include <iomanip>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <vector>

#include "client/Client.hpp"
#include "client/Parser.hpp"
#include "server/VirtualServer.hpp"

#define CHILD_BUFFER_SIZE 4096
#define IS_FILE			  0
#define IS_DIR			  1

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
	std::string			_connection_status;
	std::string			_code;
	std::vector< char > _body;

	void createErrorPage();

	void		buildHeader();
	static void trimPath(std::string &path);

	int	 verifLocationAndGetNewPath();
	bool verifAccess();
	void insertFileInHead(std::string &file, const off_t &size,
						  const std::string &date, const int &id);
	void insertFooterAndSetAttributes(std::vector< char > &body);

	bool findErrorPageLocation();
	bool findErrorPageServer();
	bool findErrorPageDefaultServer();

	void verifCGI(int &state);

  public:
	Builder(const VirtualServer *server, const VirtualServer *default_server,
			const client::Parser &parser);
	~Builder();

	void BuildResponse(std::string &response);
	void verifDenyMethod(int &state);
	void returnParam(int &state);
	void findFile();
	void readFile();
	void findErrorPage();
	void setIndexOrAutoindex(int &state);
	void getAutoindex();
	void isCGI(int &state);
	int	 readDataRequest();

	const std::string &getFileExtension() const { return _extension; };
	const std::string &getFilename() const { return _filename; };
	const std::string &getCode() const { return _code; };
	const std::string &getPath() const { return _path; };

	void setCode(const std::string &code) { _code = code; };
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
    <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css\">\
    <link href=\"https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap\" rel=\"stylesheet\">\
    <style>\
    .parent-container {display: flex; flex-wrap: wrap; justify-content: left; align-items: flex; padding: 20px;}\
    .container {width: 30%; margin: 10px; overflow: hidden; box-sizing: border-box; transition: transform 0.3s, box-shadow 0.3s; min-height:200px;}\
    .title {color: white; font-size: 2rem; text-align: left; border: 4px solid black; padding: 1rem;\
	border-radius: 10px; background-color: #EE92C2; margin-bottom: 15px; margin-top: 20px;word-wrap: break-word; overflow: hidden; text-overflow: ellipsis;}\
    .title:hover {background-color: #add8e6;}\
    .icon-file {color: #add8e6; padding-right: 10px;}\
    .icon-folder {color: #add8e6; padding-right: 10px;}\
	.title:hover .icon-file {color: #EE92C2;}\
    .title:hover .icon-folder {color: #EE92C2;}\
    .file-text {color: black; font-size: 1.5rem;}\
    .last-modification {font-size: 1rem; color: black; border-radius: 10px; background-color: #add8e6; padding: 5px; margin-top: 10px;}\
	.size {font-size: 1rem; color: black; border-radius: 10px; background-color: #add8e6; padding: 5px; margin-top: 5px;}\
	a {text-decoration: none; color: black;}\
    body {font-family: 'Roboto', sans-serif; background-color: #f0f0f0; margin: 0; padding: 0;}\
    </style>\
    <meta charset=\"UTF-8\">\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
    <title>Autoindex</title>\
</head>\
<body>\
    <div class=\"parent-container\">\
	<div class=\"container\"><div class=\"title\"><h3>Index of %@path@%</h3><br></div></div>"

#define DEFAULT_AUTOINDEX_LIST_FILE                                           \
	"<div class=\"container\"><a href=\"%@file@%\" style=\"text-decoration: " \
	"none; color: inherit;\"><div class=\"title\"><i class=\"fas fa-file "    \
	"icon-file\"></i> "                                                       \
	"<span class=\"file-text\">%@file@%</span></div></a><span "               \
	"class=\"last-modification\">Last "                                       \
	"Modification : %@last_modif@%</span><br><br><span class=\"size\">"       \
	"%@size@%\"</span></div>"

#define DEFAULT_AUTOINDEX_LIST_DIR                                            \
	"<div class=\"container\"><a href=\"%@file@%\" style=\"text-decoration: " \
	"none; color: inherit;\"><div class=\"title\"><i class=\"fas fa-folder "  \
	"icon-folder\"></i> "                                                     \
	"<span class=\"file-text\">%@file@%</span></div></a><span "               \
	"class=\"last-modification\">Last "                                       \
	"Modification : %@last_modif@%</span><br><br><span class=\"size\">"       \
	"%@size@%\"</span></div>"

#define DEFAULT_AUTOINDEX_PAGE_FOOT "</div></body></html>"

#endif
