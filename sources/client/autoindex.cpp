/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:14:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/18 11:31:07 by Monsieur_Ca      ###   ########.fr       */
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

	ret = access(new_path.c_str(), F_OK | R_OK);
	if (ret != 0) {
		_code = (errno == ENOENT) ? "404" : "403";
		findBodyErrorPage(parser, body);
		return false;
	}
	return true;
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

int Builder::verifLocationAndGetNewPath(const client::Parser &parser,
										string				 &final_path,
										std::vector< char >	 &body)
{
	string			root;
	const Location *location;
	string			autoindex;
	string			all_path;

	all_path = parser.getRequestedPath() + parser.getFilename();
	string search_location = all_path;
	findRootDirectory(all_path, final_path);

	while (search_location.size() != 0) {
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
		if (search_location.size() == 1) {
			break;
		}
		search_location = search_location.substr(0, search_location.rfind("/"));
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

void Builder::indexOrAutoindexList(const client::Parser &parser,
								   std::vector< char >	&body)
{
	DIR			  *dir;
	struct dirent *entry;
	string		   file;
	string		   final_path;

	int ret = verifLocationAndGetNewPath(parser, final_path, body);
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
