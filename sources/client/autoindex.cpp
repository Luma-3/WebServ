/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:14:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/21 14:28:49 by jbrousse         ###   ########.fr       */
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
		findErrorPage();
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
		root = location->getRoot(_request_path);
		if (!index.empty()) {
			_path = root + index;
			_filename = index;
			_extension = Parser::findExtension(index);
			return INDEX;
		}
		autoindex = location->getParamValue("autoindex");
		if (!autoindex.empty()) {
			_path = location->getRoot(_request_path);
		}
		_code = (autoindex == "on") ? _code : "403";
		return (autoindex == "on") ? AUTOINDEX : ERROR;
	}
	index = _server->getParamValue("index");
	root = _server->getParamValue("root");
	if (!index.empty()) {
		_path = root + index;
		_filename = index;
		_extension = Parser::findExtension(index);
		return INDEX;
	}
	autoindex = _server->getParamValue("autoindex");
	_path = _server->getRoot(_request_path);
	_code = (autoindex == "on") ? _code : "403";
	return (autoindex == "on") ? AUTOINDEX : ERROR;
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

void Builder::getAutoindex()
{
	struct dirent *entry;
	string		   file;
	DIR			  *dir;

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

void Builder::setIndexOrAutoindex(int &state)
{
	if (!_filename.empty()) {
		return;
	}
	state = verifLocationAndGetNewPath();
	if (state != AUTOINDEX) {
		return;
	}
	trimPath(_path);
	if (access(_path.c_str(), F_OK | R_OK) != 0) {
		state = ERROR;
		return;
	}
}