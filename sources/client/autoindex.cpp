/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:14:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/19 17:52:55 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"
#include "client/Client.hpp"

using client::Builder;
using std::string;

void Builder::trimPath(string &path)
{
	if (path[path.size() - 1] != '/') {
		path += "/";
	}
	if (path[0] == '/') {
		path = path.substr(1);
	}
}

bool Builder::verifAccess()
{
	int ret;

	ret = access(_path.c_str(), F_OK | R_OK);
	if (ret != 0) {
		_code = (errno == ENOENT) ? "404" : "403";
		findErrorpageLocationServer();
		return false;
	}
	return true;
}

int Builder::verifLocationAndGetNewPath()
{
	string			root;
	const Location *location;
	string			autoindex;
	string			index;

	location = _server->getLocation(_request_path);
	if (location != NULL) {
		index = location->getParamValue("index");
		if (!index.empty()) {
			_path = root + index;
			_filename = index;
			_extension = Parser::findExtension(index);
			return INDEX_FIND;
		}
		autoindex = location->getParamValue("autoindex");
		if (!autoindex.empty()) {
			_path = location->getRoot(_request_path);
			return (autoindex == "on") ? AUTOINDEX_ON : AUTOINDEX_OFF;
		}
		return AUTOINDEX_OFF;
	}
	index = _server->getParamValue("index");
	root = _server->getRoot(_request_path);
	if (!index.empty()) {
		_path = root + index;
		_filename = index;
		_extension = Parser::findExtension(index);
		std::cout << "Extension: " << _extension << std::endl;
		return INDEX_FIND;
	}
	autoindex = _server->getParamValue("autoindex");
	_path = _server->getRoot(_request_path);
	return (autoindex == "on") ? AUTOINDEX_ON : AUTOINDEX_OFF;
}

void Builder::insertFileInHead(string &file)
{
	string head = DEFAULT_AUTOINDEX_LIST;
	head.replace(head.find("%@file@%"), 8, file);
	head.replace(head.find("%@file@%"), 8, file);
	_body.insert(_body.end(), head.begin(), head.end());
}

void Builder::insertFooterAndSetAttributes(std::vector< char > &body)
{
	string foot = DEFAULT_AUTOINDEX_PAGE_FOOT;
	body.insert(body.end(), foot.begin(), foot.end());
	_extension = "html";
}

void Builder::setIndexOrReturnAutoindex()
{
	struct dirent *entry;
	string		   file;
	DIR			  *dir;

	std::cout << "JE SUIS DANS SET INDEX OR RETURN AUTOINDEX" << std::endl;
	if (!_filename.empty()) {
		return;
	}
	int ret = verifLocationAndGetNewPath();
	if (ret == INDEX_FIND) {
		return;
	}
	else if (ret == AUTOINDEX_OFF) {
		_code = "403";
		findErrorpageLocationServer();
		std::cout << "J'ai Trouve un autoindex sur off" << std::endl;
		return;
	}
	trimPath(_path);
	if (!verifAccess()) {
		return;
	}

	string head = DEFAULT_AUTOINDEX_PAGE_HEAD;
	_body.insert(_body.end(), head.begin(), head.end());

	dir = opendir(_path.c_str());
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.') {
			continue;
		}
		if (entry->d_type == DT_DIR) {
			file = entry->d_name + string("/");
		}
		else {
			file = entry->d_name;
		}
		insertFileInHead(file);
	}
	closedir(dir);
	insertFooterAndSetAttributes(_body);
}