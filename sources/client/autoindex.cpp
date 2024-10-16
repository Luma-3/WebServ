/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:14:53 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/16 14:45:31 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

using client::Builder;

int countOccurrences(const std::string &str, const std::string &sub)
{
	if (sub.empty()) {
		return 0;
	}
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

void Builder::verifServerAndGetNewPath(const client::Parser &parser,
									   std::string			&new_path,
									   std::vector< char >	&body)
{
	if (!_server->getParamValue("index").empty()) {
		readFile(parser,
				 _server->getParamValue("root") +
					 _server->getParamValue("index"),
				 body);
		return;
	}
	if (_server->getParamValue("autoindex") == "off") {
		_code = "403";
		findBodyErrorPage(parser, body);
		return;
	}
	new_path = _server->getParamValue("root");
	if (new_path.empty()) {
		new_path = parser.getRequestedPath() + parser.getFilename();
	}
	else if (parser.getRequestedPath() != "/") {
		new_path = parser.getRequestedPath() + parser.getFilename();
	}
}

bool Builder::verifLocationAndGetNewPath(const client::Parser &parser,
										 std::string		  &new_path,
										 std::vector< char >  &body)
{
	const Location *location = _server->getLocation(parser.getRequestedPath());
	if (location != NULL) {
		if (!location->getParamValue("index").empty()) {
			std::cout << "J;ai trouve un index" << std::endl;
			std::cout << "son path ets : "
					  << location->getParamValue("root") +
							 location->getParamValue("index")
					  << std::endl;
			readFile(parser,
					 location->getParamValue("root") +
						 location->getParamValue("index"),
					 body);
			return true;
		}
		if (location->getParamValue("autoindex") == "off") {
			_code = "403";
			findBodyErrorPage(parser, body);
			return true;
		}
		new_path = location->getParamValue("root");
		if (new_path.empty()) {
			new_path = parser.getRequestedPath() + parser.getFilename();
		}
		else if (parser.getRequestedPath() != "/") {
			new_path += parser.getRequestedPath() + parser.getFilename();
		}
		return true;
	}
	return false;
}

void Builder::returnAutoindexList(const client::Parser &parser,
								  std::vector< char >  &body)
{
	std::string new_path;
	std::string tmp;

	if (verifLocationAndGetNewPath(parser, new_path, body) == false) {
		std::cout << "Je n'ai pas trouve de location" << std::endl;
		verifServerAndGetNewPath(parser, new_path, body);
	}

	if (new_path[new_path.size() - 1] != '/') {
		new_path += "/";
	}
	if (new_path[0] == '/') {
		new_path = new_path.substr(1);
	}

	std::cout << "NEW PATH : " << new_path << std::endl;
	int ret = access(new_path.c_str(), F_OK | R_OK);
	if (ret != 0) {
		_code = (errno == ENOENT) ? "404" : "403";
		findBodyErrorPage(parser, body);
		return;
	}
	DIR *dir = opendir(new_path.c_str());

	tmp = DEFAULT_AUTOINDEX_PAGE_HEAD;
	body.insert(body.end(), tmp.begin(), tmp.end());

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		std::string html_line_file = DEFAULT_AUTOINDEX_LIST;
		if (entry->d_name[0] == '.') {
			continue;
		}
		std::string relative_path = new_path + entry->d_name;

		cutParentDirectoryInPath(relative_path);
		if (entry->d_type == DT_DIR) {
			relative_path += "/";
		}
		std::cout << "relative path : " << relative_path << std::endl;
		html_line_file.replace(html_line_file.find("%@file@%"), 8,
							   relative_path);
		html_line_file.replace(html_line_file.find("%@file@%"), 8,
							   entry->d_name);
		body.insert(body.end(), html_line_file.begin(), html_line_file.end());
	}
	closedir(dir);
	std::string foot = DEFAULT_AUTOINDEX_PAGE_FOOT;
	body.insert(body.end(), foot.begin(), foot.end());
	_content_type = "text/html";
}

bool Builder::isDirRequest(const std::string &path)
{
	if (path[path.size() - 1] == '/') {
		return true;
	}
	return false;
}