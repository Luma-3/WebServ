/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_url.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:24:36 by anthony           #+#    #+#             */
/*   Updated: 2024/10/22 14:53:09 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using std::string;
using std::vector;

void client::Parser::handleRequestedPath(string &requested_path)
{
	size_t first_dot = requested_path.find_first_of('.');
	size_t last_slash = requested_path.find_last_of('/'); // TODO base last_slash on query start pos 
	size_t next_slash = (first_dot == string::npos)
						  ? string::npos
						  : requested_path.find_first_of('/', first_dot);
	size_t query_pos = _requested_path.find('?');

	if (first_dot == string::npos || next_slash == string::npos) {
		_requested_path = requested_path.substr(0, last_slash + 1);
		_filename =
			requested_path.substr(last_slash + 1, query_pos - last_slash - 1);
		_extension =.
			requested_path.substr(first_dot + 1, query_pos - first_dot - 1);
		_query = requested_path.substr(query_pos + 1);
	}
	else {
		std::string tmp_requested_path = requested_path.substr(0, next_slash);

		last_slash = tmp_requested_path.find_last_of('/');

		_requested_path = tmp_requested_path.substr(0, last_slash + 1);
		_filename = tmp_requested_path.substr(last_slash + 1);
		_path_info =
			requested_path.substr(next_slash + 1, query_pos - next_slash - 1);
		_extension = _filename.substr(_filename.find_last_of('.') + 1);
		if (query_pos != string::npos) {
			_query = requested_path.substr(query_pos + 1);
		}
	}

	std::cout << "Requested path: " << _requested_path << std::endl;
	std::cout << "Filename: " << _filename << std::endl;
	std::cout << "CGI path info: " << _path_info << std::endl;
}