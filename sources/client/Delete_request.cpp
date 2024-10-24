/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:15:23 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/24 10:44:53 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"

using client::Client;

bool Client::verifAccessInsideDirectory(const std::string &full_path)
{
	DIR			  *dir;
	struct dirent *entry;

	std::cout << "Verifying access inside " << full_path << std::endl;
	dir = opendir(full_path.c_str());
	if (dir == NULL) {
		_builder->setCode("500");
		return false;
	}
	while ((entry = readdir(dir)) != NULL) {
		std::string path_to_file = full_path + "/" + entry->d_name;

		if (strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		struct stat infos;

		if (access(path_to_file.c_str(), F_OK | W_OK) == -1) {
			_builder->setCode("403");
			closedir(dir);
			return false;
		}
		if (stat(path_to_file.c_str(), &infos) == -1) {
			_builder->setCode("500");
			return false;
		}
		if (S_ISDIR(infos.st_mode)) {
			if (verifAccessInsideDirectory(path_to_file) == false) {
				closedir(dir);
				return false;
			}
		}
	}
	closedir(dir);
	return true;
}

void Client::removeDirectory(const std::string &full_path)
{
	DIR			  *dir;
	struct dirent *entry;

	dir = opendir(full_path.c_str());
	if (dir == NULL) {
		_builder->setCode("500");
		return;
	}
	while ((entry = readdir(dir)) != NULL) {
		std::string path_to_file = full_path + "/" + entry->d_name;

		if (strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		struct stat infos;
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
		_builder->setCode("500");
	}
}

void Client::removeFile(const std::string &full_path)
{
	std::cout << "Removing file " << full_path << std::endl;
	if (unlink(full_path.c_str()) == -1) {
		_builder->setCode("500");
		return;
	}
}

void Client::handleDeleteRequest(const Parser &parser)
{
	std::string path = parser.getRequestedPath();
	std::string root = _server->getRoot(path);
	std::string filename = parser.getFilename();
	std::string full_path = root + filename;

	if (full_path[full_path.size() - 1] == '/') {
		full_path = full_path.substr(0, full_path.size() - 1);
	}
	int ret = access(full_path.c_str(), F_OK | W_OK);
	if (ret != 0) {
		(errno == EACCES) ? _builder->setCode("403") : _builder->setCode("404");
		return;
	}
	struct stat infos;
	if (stat(full_path.c_str(), &infos) == -1) {
		_builder->setCode("500");
		return;
	}
	if (S_ISDIR(infos.st_mode)) {
		if (verifAccessInsideDirectory(full_path) == false) {
			return;
		}
		removeDirectory(full_path);
		return;
	}
	else {
		removeFile(full_path);
		return;
	}
	_builder->setCode("204");
}