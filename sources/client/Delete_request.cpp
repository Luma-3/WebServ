/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:15:23 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/11/27 11:05:57 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "client/Client.hpp"
#include "finder.hpp"

using client::Client;

namespace {

bool isCurrentOrBackDir(const char *entry_name)
{
	return (strncmp(entry_name, ".", strlen(entry_name)) == 0 ||
			strncmp(entry_name, "..", strlen(entry_name)) == 0);
}
} // namespace

bool Client::verifAccessInsideDirectory(const std::string &full_path)
{
	DIR			  *dir = NULL;
	struct dirent *entry = NULL;
	struct stat	   infos = {};
	std::string	   path_to_file;

	dir = opendir(full_path.c_str());
	if (dir == NULL) {
		LOG_WARNING("Open directory error: " + std::string(strerror(errno)));
		_builder->setCode("500");
		return false;
	}

	while ((entry = readdir(dir)) != NULL) {

		path_to_file = full_path + "/" + entry->d_name;
		memset(&infos, 0, sizeof(infos));

		if (isCurrentOrBackDir(entry->d_name)) {
			continue;
		}

		if (access(path_to_file.c_str(), F_OK | W_OK) == -1) {
			LOG_WARNING("Accessing to" + path_to_file +
						": error: " + std::string(strerror(errno)));
			(errno == ENOENT) ? _builder->setCode("404")
							  : _builder->setCode("403");
			closedir(dir);
			return false;
		}
		if (stat(path_to_file.c_str(), &infos) == -1) {
			LOG_ERROR("Stat error: " + std::string(strerror(errno)));
			_builder->setCode("500");
			closedir(dir);
			return false;
		}

		if (S_ISDIR(infos.st_mode) &&
			!verifAccessInsideDirectory(path_to_file)) {
			closedir(dir);
			_builder->setCode("403");
			return false;
		}
	}
	closedir(dir);
	return true;
}

void Client::removeDirectory(const std::string &full_path)
{
	DIR			  *dir = NULL;
	struct dirent *entry = NULL;
	std::string	   path_to_file;
	struct stat	   infos = {};

	dir = opendir(full_path.c_str());
	if (dir == NULL) {
		_builder->setCode("500");
		return;
	}
	while ((entry = readdir(dir)) != NULL) {
		memset(&infos, 0, sizeof(infos));
		path_to_file = full_path + "/" + entry->d_name;

		if (isCurrentOrBackDir(entry->d_name)) {
			continue;
		}
		if (stat(path_to_file.c_str(), &infos) == -1) {
			LOG_WARNING("Stat error: " + std::string(strerror(errno)));
			_builder->setCode("500");
			closedir(dir);
			return;
		}
		if (S_ISDIR(infos.st_mode)) {
			removeDirectory(path_to_file);
		}
		else {
			removeFile(path_to_file);
		}
	}
	closedir(dir);
	if (rmdir(full_path.c_str()) == -1) {
		LOG_WARNING("Rmdir error: " + std::string(strerror(errno)));
		_builder->setCode("500");
	}
}

void Client::removeFile(const std::string &full_path)
{
	if (unlink(full_path.c_str()) == -1) {
		_builder->setCode("500");
		return;
	}
}

void Client::handleDeleteRequest()
{

	std::string full_path = findRoot(_builder->getRequestedPath(), _server) +
							_builder->getFilename();
	struct stat infos = {};

	if (full_path[full_path.size() - 1] == '/') {
		full_path = full_path.substr(0, full_path.size() - 1);
	}

	if (access(full_path.c_str(), F_OK | W_OK) != 0) {
		LOG_INFO("Accessing to " + full_path +
				 ": error: " + std::string(strerror(errno)));
		(errno == EACCES) ? _builder->setCode("403") : _builder->setCode("404");
		return;
	}

	if (stat(full_path.c_str(), &infos) == -1) {
		LOG_WARNING("Stat error: " + std::string(strerror(errno)));
		_builder->setCode("500");
		return;
	}

	if (S_ISDIR(infos.st_mode)) {
		if (!verifAccessInsideDirectory(full_path)) {
			return;
		}
		removeDirectory(full_path);
		return;
	}
	removeFile(full_path);
}
