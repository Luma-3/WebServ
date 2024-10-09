/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_url.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:24:36 by anthony           #+#    #+#             */
/*   Updated: 2024/10/08 14:53:04 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using std::string;
using std::vector;

string client::Parser::getConfigParam(const string &param,
									  IdentifyParam identify)
{
	typedef string (client::Parser::*MemberFunction)(const std::string &,
													 IdentifyParam);
	MemberFunction function[5] = {&client::Parser::find_param_location,
								  &client::Parser::find_param_server,
								  &client::Parser::find_param_location_default,
								  &client::Parser::find_param_default_server,
								  &client::Parser::find_hard_code};
	string		   found_value;
	for (size_t i = 0; i < 5; i++) {
		found_value = (this->*function[i])(param, identify);
		if (found_value != "") return found_value;
	}
	return "";
}

// void ConstructPath(string &path, string &filename, string &file_extension)
// {
// 	statement::Location *location =
// }

void client::Parser::handleRequestedPath(string &requested_path)
{
	size_t		last_slash = requested_path.find_last_of('/');
	std::string file;

	_requested_path = requested_path.substr(0, last_slash + 1);

	_filename = requested_path.substr(last_slash + 1);
	if (_filename.empty()) {
		_filename = getConfigParam(_requested_path, P_INDEX);
	}
	size_t last_dot = _filename.find_last_of('.');

	_file_extension =
		last_dot == string::npos ? "" : _filename.substr(last_dot + 1);

	_path = getConfigParam(_requested_path, P_ROOT);
}