/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:14:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/17 18:14:14 by anthony          ###   ########.fr       */
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

void Builder::findRootDirectory(const string &path, string &root)
{
	const Location *location;
	string			tmp = path;

	while (tmp.size() != 0) {
		std::cout << "JE cherche le root pour : " << tmp << std::endl;
		location = _server->getLocation(tmp);
		if (location != NULL) {
			root = location->getRoot(path);
			if (root.empty() == false) {
				return;
			}
		}
		if (tmp.size() == 1) {
			break;
		}
		tmp = tmp.substr(tmp.rfind("/"));
	}
	root = _server->getParamValue("root");
}

int Builder::findIndexOrAutoindexConfig(const client::Parser &parser,
										const std::string	 &all_path,
										const std::string	 &final_path,
										std::vector< char >	 &body)
{
	const Location *location;
	string			directory;
	string			autoindex;
	int				pos;
	string			tmp;

	tmp = all_path;
	while (tmp.size() != 0) {

		pos = tmp.rfind("/", all_path.size() - 2);
		directory = tmp.substr(pos + 1);
		tmp = tmp.substr(0, pos);
		location = _server->getLocation(directory);
		std::cout << "Location : " << all_path << std::endl;
		if (location != NULL) {
			if (location->getParamValue("index").empty() == false) {
				readFile(parser, final_path + location->getParamValue("index"),
						 body);
				return INDEX_FIND;
			}
			autoindex = location->getParamValue("autoindex");
			if (autoindex.empty() == false) {
				return (autoindex == "on") ? AUTOINDEX_ON : AUTOINDEX_OFF;
			}
		}
	}
	if (_server->getParamValue("index").empty() == false) {
		readFile(parser, final_path + _server->getParamValue("index"), body);
		return INDEX_FIND;
	}
	autoindex = _server->getParamValue("autoindex");
	if (autoindex.empty() == false) {
		return (autoindex == "on") ? AUTOINDEX_ON : AUTOINDEX_OFF;
	}
	return AUTOINDEX_OFF;
}

int Builder::verifLocationAndGetNewPath(const client::Parser &parser,
										const string		 &search_location,
										string				 &final_path,
										std::vector< char >	 &body)
{
	string			root;
	const Location *location;
	string			autoindex;
	string			all_path;

	all_path = parser.getRequestedPath() + parser.getFilename();

	location = _server->getLocation(all_path);
	final_path = location->getRoot(all_path);
	if (final_path.empty() == true) {
		findRootDirectory(all_path, final_path);
	}

	location = _server->getLocation(search_location);
	if (location != NULL) {
		if (location->getParamValue("index").empty() == false) {
			readFile(parser, final_path + location->getParamValue("index"),
					 body);
			return INDEX_FIND;
		}
		autoindex = location->getParamValue("autoindex");
		if (autoindex.empty() == false) {
			return (autoindex == "on") ? AUTOINDEX_ON : AUTOINDEX_OFF;
		}
	}
	// std::cout << "Je n'ai pas trouve de location pour : " << search_location
	// 		  << std::endl;
	// autoindex = getConfigIndexOrFindAutoindex(final_path, parser, body);
	// if (autoindex == "AUTOINDEX ON") {
	// 	return AUTOINDEX_ON;
	// }
	// else if (autoindex == "INDEX_FIND") {
	// 	return INDEX_FIND;
	// }
	return AUTOINDEX_OFF;
}

void Builder::insertFileInHead(string &file, string &head,
							   std::vector< char > &body)
{
	head.replace(head.find("%@file@%"), 8, file);
	head.replace(head.find("%@file@%"), 8, file);
	body.insert(body.end(), head.begin(), head.end());
}

void Builder::insertFooterAndSetAttributes(std::vector< char > &body)
{
	string foot = DEFAULT_AUTOINDEX_PAGE_FOOT;
	body.insert(body.end(), foot.begin(), foot.end());
	_content_type = "text/html";
}

string Builder::cutParentDirectoryInPath(const string &path)
{
	string new_path;

	new_path = path;
	if (path.size() == 1) {
		return new_path;
	}
	size_t pos = new_path.rfind("/", new_path.size() - 2);
	if (pos != 0) {
		new_path = new_path.substr(pos);
	}
	return new_path;
}

void Builder::indexOrAutoindexList(const client::Parser &parser,
								   std::vector< char >	&body)
{
	DIR			  *dir;
	struct dirent *entry;
	string		   file;
	string		   search_location;
	string		   final_path;

	search_location = cutParentDirectoryInPath(parser.getRequestedPath() +
											   parser.getFilename());
	int ret =
		verifLocationAndGetNewPath(parser, search_location, final_path, body);
	if (ret == INDEX_FIND) {
		return;
	}
	else if (ret == AUTOINDEX_OFF) {
		_code = "403";
		findBodyErrorPage(parser, body);
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
	insertFooterAndSetAttributes(body);
}
