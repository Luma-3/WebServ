/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:14:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/17 13:29:32 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"
#include "client/Client.hpp"

using client::Builder;
using std::string;

bool Builder::isDirRequest(const string &path)
{
	if (path[path.size() - 1] == '/') {
		return true;
	}
	return false;
}

void Builder::trimPath(string &path)
{
	if (path[path.size() - 1] != '/') {
		path += "/";
	}
	if (path[0] == '/') {
		path = path.substr(1);
	}
}

bool Builder::verifAccess(const client::Parser &parser, const string &new_path,
						  std::vector< char > &body)
{
	int ret;

	std::cout << "Je verifie l'acces au fichier : " << new_path << std::endl;
	ret = access(new_path.c_str(), F_OK | R_OK);
	if (ret != 0) {
		_code = (errno == ENOENT) ? "404" : "403";
		findBodyErrorPage(parser, body);
		return false;
	}
	return true;
}

string Builder::getConfigRoot(const string &path)
{
	string			root;
	const Location *location;

	location = _server->getLocation(path);
	if (location != NULL) {
		root = location->getParamValue("root");
		if (root.empty() == false) {
			return root;
		}
	}
	location = _server->getLocation("/");
	if (location != NULL) {
		root = location->getParamValue("root") + path;
	}
	else {
		root = _server->getParamValue("root") + path;
	}
	std::cout << "La root pour le dossier demande est : " << root << std::endl;
	return root;
}

string Builder::getConfigIndexOrFindAutoindex(const string		   &root,
											  const client::Parser &parser,
											  std::vector< char >  &body)
{
	string			autoindex;
	const Location *location;

	location = _server->getLocation("/");
	if (location != NULL) {
		if (location->getParamValue("index").empty() == false) {
			readFile(parser, root + location->getParamValue("index"), body);
			return "INDEX_FIND";
		}
		autoindex = location->getParamValue("autoindex");
		if (autoindex.empty()) {
			if (_server->getParamValue("index").empty() == false) {
				readFile(parser, root + _server->getParamValue("index"), body);
				return "INDEX_FIND";
			}
		}
		return autoindex;
	}
	if (_server->getParamValue("index").empty() == false) {
		readFile(parser, root + _server->getParamValue("index"), body);
		return "INDEX_FIND";
	}
	autoindex = _server->getParamValue("autoindex");
	if (autoindex.empty() == true) {
		_code = "403";
		findBodyErrorPage(parser, body);
		return "off";
	}
	return autoindex;
}

int Builder::verifLocationAndGetNewPath(const client::Parser &parser,
										const string		 &search_location,
										string				 &final_path,
										std::vector< char >	 &body)
{
	string			root;
	const Location *location;
	string			autoindex;

	root = getConfigRoot(search_location);
	location = _server->getLocation(search_location);
	if (location != NULL) {

		if (location->getParamValue("index").empty() == false) {
			readFile(parser, root + location->getParamValue("index"), body);
			return INDEX_FIND;
		}
		autoindex = location->getParamValue("autoindex");
	}
	if (autoindex.empty()) {
		autoindex = getConfigIndexOrFindAutoindex(root, parser, body);
		if (autoindex == "INDEX_FIND") {
			return INDEX_FIND;
		}
	}
	if (autoindex != "on") {
		_code = "403";
		findBodyErrorPage(parser, body);
		return AUTOINDEX_OFF;
	}
	final_path = root;
	return AUTOINDEX_ON;
}

void Builder::insertFileInHead(string &file, string &head,
							   std::vector< char > &body)
{
	head.replace(head.find("%@file@%"), 8, file);
	head.replace(head.find("%@file@%"), 8, file);
	body.insert(body.end(), head.begin(), head.end());
}

void Builder::insertFooterAndSetAttributes(client::Client	   *client,
										   std::vector< char > &body)
{
	string foot = DEFAULT_AUTOINDEX_PAGE_FOOT;
	body.insert(body.end(), foot.begin(), foot.end());

	_content_type = "text/html";
	client->setAutoindexParentLocation(true);
}

string Builder::cutParentDirectoryInPath(const string &path)
{
	string new_path;

	new_path = path;
	size_t pos = path.find_first_of("/", 1);
	if (pos != path.size() - 1) {
		new_path = "/" + path.substr(pos + 1);
	}
	return new_path;
}

void Builder::indexOrAutoindexList(const client::Parser &parser,
								   std::vector< char >	&body,
								   client::Client		*client)
{
	DIR			  *dir;
	struct dirent *entry;
	string		   path;
	string		   file;
	string		   search_path;
	string		   final_path;

	search_path = cutParentDirectoryInPath(parser.getRequestedPath() +
										   parser.getFilename());
	std::cout << "search_path : " << search_path << std::endl;
	int ret = verifLocationAndGetNewPath(parser, search_path, final_path, body);
	if (ret == INDEX_FIND) {
		std::cout << "Je trouve un index" << std::endl;
		return;
	}
	else if (ret == AUTOINDEX_OFF) {
		std::cout << "Je trouve un autoindex off" << std::endl;
		client->setAutoindexParentLocation(false);
		return;
	}
	trimPath(final_path);
	if (verifAccess(parser, final_path, body) == false) {
		return;
	}

	string head = DEFAULT_AUTOINDEX_PAGE_HEAD;
	body.insert(body.end(), head.begin(), head.end());

	dir = opendir(final_path.c_str());
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.') {
			continue;
		}
		head = DEFAULT_AUTOINDEX_LIST;

		if (entry->d_type == DT_DIR) {
			file = entry->d_name + string("/");
		}
		else {
			file = entry->d_name;
		}
		std::cout << "file : " << file << std::endl;
		insertFileInHead(file, head, body);
	}
	closedir(dir);
	insertFooterAndSetAttributes(client, body);
}
