/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser_url.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:24:36 by anthony           #+#    #+#             */
/*   Updated: 2024/10/11 14:34:58 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Parser.hpp"

using std::string;
using std::vector;

void client::Parser::handleRequestedPath(string &requested_path)
{
	s_info_param info;
	size_t		 last_slash = requested_path.find_last_of('/');
	std::string	 file;
	bool		 found_param;

	_requested_path = requested_path.substr(0, last_slash + 1);
	_filename = requested_path.substr(last_slash + 1);

	found_param = getConfigParam(info, F_ROOT, F_INDEX);
	if (found_param) {
		_path = info.root;
		std::cout << "path: " << _path << std::endl;
		if (_filename.empty()) {
			std::cout << "je sui la" << std::endl;
			_filename = info.index;
		}
		std::cout << "filename: " << _filename << std::endl;
	}
	else {
		_path = "";
	}
}