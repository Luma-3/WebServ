/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_url.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:24:36 by anthony           #+#    #+#             */
/*   Updated: 2024/10/10 14:40:49 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using std::string;
using std::vector;

std::pair< string, string >
client::Parser::getConfigParam(const string &param, IdentifyParam identify)
{
	typedef string (client::Parser::*MemberFunction)(const std::string &,
													 IdentifyParam);
	MemberFunction function[4] = {&client::Parser::find_param_location,
								  &client::Parser::find_param_server,
								  &client::Parser::find_param_location_default,
								  &client::Parser::find_param_default_server};
	string		   found_value;
	string		   found_root;
	for (size_t i = 0; i < 4; i++) {
		found_value = (this->*function[i])(param, identify);
		if (found_value != "") {
			found_root = (this->*function[i])(param, P_ROOT);
			return std::make_pair(found_value, found_root);
		}
	}
	return std::make_pair("", "");
}

void client::Parser::handleRequestedPath(string &requested_path)
{
	size_t						last_slash = requested_path.find_last_of('/');
	std::string					file;
	std::pair< string, string > found_param;

	_requested_path = requested_path.substr(0, last_slash + 1);

	_filename = requested_path.substr(last_slash + 1);
	if (_filename.empty()) {
		found_param = getConfigParam(_requested_path, P_INDEX);
		_filename = found_param.first;
	}
	found_param = getConfigParam(_requested_path, P_ROOT);
	_path = found_param.first;
}