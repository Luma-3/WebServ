/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_url.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:24:36 by anthony           #+#    #+#             */
/*   Updated: 2024/09/24 15:17:44 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

string client::Parser::extractPathAndFilename(string	   &url,
											  const size_t &l_slash,
											  const size_t &l_dot)
{
	string new_path;

	if (l_slash == string::npos) {
		new_path = "/";
		_filename = url;
	} else {
		new_path = url.substr(0, l_slash);
		_filename = url.substr(l_slash + 1, l_dot - l_slash - 1);
	}

	if (new_path.empty()) {
		new_path = "/";
	} else if (new_path[0] != '/') {
		new_path = "/" + new_path;
	}

	if (l_dot != string::npos && l_dot > l_slash) {
		_filename = url.substr(l_slash + 1, l_dot - l_slash - 1);
	}

	if (_filename.empty()) {
		_filename = "index";
	}
	return new_path;
}

string client::Parser::extractExtension(string &url, const size_t &l_dot)
{
	string new_extension;

	if (l_dot == string::npos) {
		new_extension = "html";
	} else {
		new_extension = url.substr(l_dot + 1);
	}
	return (new_extension);
}

void client::Parser::handleUrl(string &url)
{
	size_t last_slash = url.find_last_of('/');
	size_t last_dot = url.find_last_of('.');

	_url_path = extractPathAndFilename(url, last_slash, last_dot);

	_file_extension = extractExtension(url, last_dot);
}