/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_uri.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:24:36 by anthony           #+#    #+#             */
/*   Updated: 2024/11/18 15:54:15 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using std::string;

void client::Parser::parseURI(string &uri)
{
	const size_t query_pos = uri.find('?');
	if (query_pos != std::string::npos) {
		_query = uri.substr(query_pos + 1);
		uri = uri.substr(0, query_pos);
	}

	const size_t first_dot = uri.find_first_of('.');
	size_t		 last_slash = uri.find_last_of('/');
	const size_t next_slash = (first_dot == string::npos)
								? string::npos
								: uri.find_first_of('/', first_dot);

	if (first_dot == string::npos || next_slash == string::npos) {
		_requested_path = uri.substr(0, last_slash + 1);
		_filename = uri.substr(last_slash + 1);
		_extension = uri.substr(first_dot + 1);
	}
	else {
		const std::string tmp_uri = uri.substr(0, next_slash);

		last_slash = tmp_uri.find_last_of('/');

		_filename = tmp_uri.substr(last_slash + 1);
		_requested_path = tmp_uri.substr(0, last_slash + 1);
		_extension = _filename.substr(_filename.find_last_of('.') + 1);
		_path_info = uri.substr(next_slash + 1, query_pos - next_slash - 1);
	}
}