/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:15:23 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/11/13 09:43:39 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

using client::Client;

bool Client::verifAccessInsideDirectory(const std::string &full_path)
{
	DIR			  *dir = NULL;
	struct dirent *entry = NULL;
	;

	dir = opendir(full_path.c_str());
	if (dir == NULL) {
		LOG_WARNING("Open directory error: " + std::string(strerror(errno)),
					CSERVER);
		_builder->setCode("500");
		return false;
	}
	while ((entry = readdir(dir)) != NULL) {
		const std::string path_to_file = full_path + "/" + entry->d_name;

		if (strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		struct stat infos = {};

		if (access(path_to_file.c_str(), F_OK | W_OK) == -1) {
			LOG_INFO("Accessing to" + path_to_file +
						 ": error: " + std::string(strerror(errno)),
					 CSERVER);
			_builder->setCode("403");
			closedir(dir);
			return false;
		}
		if (stat(path_to_file.c_str(), &infos) == -1) {
			LOG_WARNING("Stat error: " + std::string(strerror(errno)), CSERVER);
			_builder->setCode("500");
			return false;
		}
		if (S_ISDIR(infos.st_mode)) {
			if (!verifAccessInsideDirectory(path_to_file)) {
				closedir(dir);
				_builder->setCode("403");
				return false;
			}
		}
	}
	closedir(dir);
	return true;
}

void Client::removeDirectory(const std::string &full_path)
{
	DIR			  *dir = NULL;
	struct dirent *entry = NULL;

	dir = opendir(full_path.c_str());
	if (dir == NULL) {
		_builder->setCode("500");
		return;
	}
	while ((entry = readdir(dir)) != NULL) {
		const std::string path_to_file = full_path + "/" + entry->d_name;

		if (strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		struct stat infos = {};
		if (stat(path_to_file.c_str(), &infos) == -1) {
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
		LOG_WARNING("Rmdir error: " + std::string(strerror(errno)), CSERVER);
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

// TODO : check conditionnal logic

void Client::handleDeleteRequest()
{
	std::string full_path = _builder->getPath();

	if (full_path[full_path.size() - 1] == '/') {
		full_path = full_path.substr(0, full_path.size() - 1);
	}
	const int ret = access(full_path.c_str(), F_OK | W_OK);
	if (ret != 0) {
		LOG_INFO("Accessing to" + full_path +
					 ": error: " + std::string(strerror(errno)),
				 CSERVER);
		(errno == EACCES) ? _builder->setCode("403") : _builder->setCode("404");
		return;
	}
	struct stat infos = {};
	if (stat(full_path.c_str(), &infos) == -1) {
		LOG_WARNING("Stat error: " + std::string(strerror(errno)), CSERVER);
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