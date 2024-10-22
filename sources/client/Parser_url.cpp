/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_url.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:24:36 by anthony           #+#    #+#             */
/*   Updated: 2024/10/22 16:52:50 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using std::string;
using std::vector;

void client::Parser::handleRequestedPath(string &requested_path)
{
	size_t query_pos = requested_path.find('?');
	if (query_pos != std::string::npos) {
		_query = requested_path.substr(query_pos + 1);
		requested_path = requested_path.substr(0, query_pos);
	}

	size_t first_dot = requested_path.find_first_of('.');
	size_t last_slash = requested_path.find_last_of('/');
	size_t next_slash = (first_dot == string::npos)
						  ? string::npos
						  : requested_path.find_first_of('/', first_dot);

	if (first_dot == string::npos || next_slash == string::npos) {
		_requested_path = requested_path.substr(0, last_slash + 1);
		_filename = requested_path.substr(last_slash + 1);
		_extension = requested_path.substr(first_dot + 1);
	}
	else {
		std::string tmp_requested_path = requested_path.substr(0, next_slash);

		last_slash = tmp_requested_path.find_last_of('/');

		_requested_path = tmp_requested_path.substr(0, last_slash + 1);
		_filename = tmp_requested_path.substr(last_slash + 1);
		_path_info =
			requested_path.substr(next_slash + 1, query_pos - next_slash - 1);
		_extension = _filename.substr(_filename.find_last_of('.') + 1);
	}

	std::cout << "Requested path: " << _requested_path << std::endl;
	std::cout << "Filename: " << _filename << std::endl;
	std::cout << "CGI path info: " << _path_info << std::endl;
	std::cout << "Query: " << _query << std::endl;
}