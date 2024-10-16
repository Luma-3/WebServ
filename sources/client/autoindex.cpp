/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:14:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/16 18:36:49 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"
#include "client/Client.hpp"

using client::Builder;

int countOccurrences(const std::string &str, const std::string &sub)
{
	int	   count = 0;
	size_t pos = 0;

	while ((pos = str.find(sub, pos)) != std::string::npos) {
		++count;
		pos += sub.length();
	}
	return count;
}

void Builder::cutParentDirectoryInPath(std::string &path)
{
	if (countOccurrences(path, "/") < 2) {
		return;
	}
	size_t pos = path.find_last_of('/');
	if (pos == std::string::npos) {
		return;
	}
	path = path.substr(pos + 1);
}

int Builder::verifLocationAndGetNewPath(const client::Parser &parser,
										std::string			 &new_path,
										std::vector< char >	 &body,
										const std::string	 &server_root)
{
	std::string search_location;
	std::string all_path = parser.getRequestedPath() + parser.getFilename();
	std::cout << "All path before : " << all_path << std::endl;
	if (all_path.find(server_root) != std::string::npos) {
		search_location =
			all_path.substr(all_path.find(server_root), all_path.size());
	}
	else {
		search_location = parser.getRequestedPath() + parser.getFilename();
	}
	std::cout << "All path after : " << search_location << std::endl;

	std::cout << " Je suis a la recherche de la location : " << search_location
			  << std::endl;
	const Location *location = _server->getLocation(search_location);
	if (location != NULL) {
		if (!location->getParamValue("index").empty()) {
			readFile(parser,
					 location->getParamValue("root") +
						 location->getParamValue("index"),
					 body);
			return INDEX_FIND;
		}
		std::string autoindex = location->getParamValue("autoindex");
		if (autoindex.empty()) {
			return NONE;
		}
		if (autoindex == "off") {
			_code = "403";
			findBodyErrorPage(parser, body);
			return AUTOINDEX_OFF;
		}
		new_path = location->getParamValue("root");
		if (new_path.empty()) {
			new_path = _server->getParamValue("root") + search_location;
		}

		return AUTOINDEX_ON;
	}
	std::cout << "J'ai pas trouver de location" << std::endl;
	return NONE;
}

bool Builder::verifServerAndGetNewPath(const client::Parser &parser,
									   std::string			&new_path,
									   std::vector< char >	&body)
{
	std::cout << "Je verifie le server" << std::endl;
	if (!_server->getParamValue("index").empty()) {
		readFile(parser,
				 _server->getParamValue("root") +
					 _server->getParamValue("index"),
				 body);
		return true;
	}
	std::string autoindex = _server->getParamValue("autoindex");
	if (autoindex.empty() || autoindex == "off") {
		_code = "403";
		findBodyErrorPage(parser, body);
		return true;
	}
	new_path = _server->getParamValue("root");
	if (new_path.empty()) {
		new_path = parser.getRequestedPath() + parser.getFilename();
	}
	else if (parser.getRequestedPath() != "/") {
		new_path = parser.getRequestedPath() + parser.getFilename();
	}
	return false;
}

bool Builder::findAndGetIndexAndAutoindexConfig(const client::Parser &parser,
												std::string			 &new_path,
												std::vector< char >	 &body,
												client::Client		 *client)
{
	bool ret2;
	int	 ret;

	ret2 = false;
	ret = verifLocationAndGetNewPath(parser, new_path, body,
									 _server->getParamValue("root"));
	switch (ret) {
		case INDEX_FIND:
			return true;
		case AUTOINDEX_OFF: {
			client->setAutoindexParentLocation(false);
			return true;
		}
		case AUTOINDEX_ON:
			return false;
		case NONE:
			if (client->getAutoindexParentLocation() == false) {
				ret2 = verifServerAndGetNewPath(parser, new_path, body);
				return ret2;
			}
	}
	new_path = parser.getRequestedPath() + parser.getFilename();
	return false;
}

void Builder::trimPath(std::string &path)
{
	if (path[path.size() - 1] != '/') {
		path += "/";
	}
	if (path[0] == '/') {
		path = path.substr(1);
	}
}

void Builder::returnAutoindexList(const client::Parser &parser,
								  std::vector< char >  &body,
								  client::Client	   *client)
{
	struct dirent *entry;
	DIR			  *dir;
	std::string	   relative_path;
	std::string	   new_path;
	std::string	   tmp;

	if (findAndGetIndexAndAutoindexConfig(parser, new_path, body, client) ==
		true) {
		std::cout << "J'ai trouver un fichier deja pas besoin de faire la liste"
				  << std::endl;
		return;
	}
	trimPath(new_path);

	std::cout << "NEW PATH : " << new_path << std::endl;
	int ret = access(new_path.c_str(), F_OK | R_OK);
	if (ret != 0) {
		_code = (errno == ENOENT) ? "404" : "403";
		findBodyErrorPage(parser, body);
		return;
	}
	tmp = DEFAULT_AUTOINDEX_PAGE_HEAD;
	body.insert(body.end(), tmp.begin(), tmp.end());

	dir = opendir(new_path.c_str());
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.') {
			continue;
		}
		tmp = DEFAULT_AUTOINDEX_LIST;
		relative_path = new_path + entry->d_name;

		cutParentDirectoryInPath(relative_path);
		if (entry->d_type == DT_DIR) {
			relative_path += "/";
		}
		std::cout << "relative path : " << relative_path << std::endl;
		tmp.replace(tmp.find("%@file@%"), 8, relative_path);
		tmp.replace(tmp.find("%@file@%"), 8, entry->d_name);
		body.insert(body.end(), tmp.begin(), tmp.end());
	}
	closedir(dir);
	std::string foot = DEFAULT_AUTOINDEX_PAGE_FOOT;
	body.insert(body.end(), foot.begin(), foot.end());
	_content_type = "text/html";
	client->setAutoindexParentLocation(true);
}

bool Builder::isDirRequest(const std::string &path)
{
	if (path[path.size() - 1] == '/') {
		return true;
	}
	return false;
}