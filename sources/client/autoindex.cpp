/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:14:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/11/04 16:11:58 by jbrousse         ###   ########.fr       */
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
	int ret = 0;

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
	const Location *location = NULL;
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
		return (autoindex == "on") ? AUTOINDEX : B_ERROR;
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
	return (autoindex == "on") ? AUTOINDEX : B_ERROR;
}

std::string formatSize(off_t size)
{
	const char		  *sizes[] = {"B", "KB", "MB", "GB"};
	int				   order = 0;
	double			   formattedSize = static_cast< double >(size);
	std::ostringstream out;
	int				   packet_size = 1024;

	while (formattedSize >= packet_size && order < 3) {
		order++;
		formattedSize /= packet_size;
	}

	out << std::fixed << std::setprecision(2) << formattedSize << " "
		<< sizes[order];
	return out.str();
}

void Builder::insertFileInHead(string &file, const off_t &size,
							   const string &date, const int &id)
{
	string head;
	string fileStr = "%@file@%";
	string last_modif = "%@last_modif@%";
	string sizeStr = "%@size@%";

	if (id == IS_FILE) {
		head = DEFAULT_AUTOINDEX_LIST_FILE;
	}
	else {
		head = DEFAULT_AUTOINDEX_LIST_DIR;
	}
	head.replace(head.find(fileStr), fileStr.size(), file);
	head.replace(head.find(fileStr), fileStr.size(), file);
	head.replace(head.find(last_modif), last_modif.size(), date);
	head.replace(head.find(sizeStr), sizeStr.size(),
				 ToString(formatSize(size)));
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
	struct dirent *entry = NULL;
	struct stat	   info = {};
	string		   file;
	DIR			  *dir = NULL;
	int			   id = 0;
	string		   path = "%@path@%";

	string head = DEFAULT_AUTOINDEX_PAGE_HEAD;
	head.replace(head.find(path), path.size(), _path);
	_body.insert(_body.end(), head.begin(), head.end());

	dir = opendir(_path.c_str());
	while ((entry = readdir(dir)) != NULL) {
		id = IS_FILE;
		if (entry->d_name[0] == '.') {
			continue;
		}
		string full_path = _path + entry->d_name;
		if (stat(full_path.c_str(), &info) == -1) {
			LOG_INFO("Impossible to get file info from " + full_path, _server);
			continue;
		}
		off_t	   size = info.st_size;
		time_t	   last_modif = info.st_mtime;
		struct tm *time = gmtime(&last_modif);
		char	   date[MAXBUFFERSIZE];
		strftime(date, MAXBUFFERSIZE, "%d-%m-%Y %H:%M", time);

		if (entry->d_type == DT_DIR) {
			file = entry->d_name + string("/");
			id = IS_DIR;
		}
		else {
			file = entry->d_name;
		}
		insertFileInHead(file, size, date, id);
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
		_code = (errno == ENOENT) ? "404" : "403";
		LOG_WARNING("accessing file " + _path + " failed", current);
		state = B_ERROR;
		return;
	}
}