/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finderConfigParam.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:50:17 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/11 14:37:59 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"
#include "client/Parser.hpp"

using client::IdentifyParam;
using client::Parser;
using std::string;

bool client::Parser::getConfigParam(s_info_param &info, int flags, int annexes,
									int start)
{
	typedef bool (client::Parser::*MemberFunction)(s_info_param &, int, int);
	MemberFunction function[4] = {&client::Parser::find_param_location,
								  &client::Parser::find_param_server,
								  &client::Parser::find_param_location_default,
								  &client::Parser::find_param_server_default};
	bool		   ret = false;
	while (start < 4) {
		ret = (this->*function[start])(info, flags, annexes);
		++start;
		if (ret) return (ret);
	}
	return (ret);
}

const statement::Location *Parser::find_location(const Server *server)
{
	if (server == NULL) {
		return NULL;
	}
	std::vector< const statement::Location * >::const_iterator it =
		server->getLocations().begin();

	while (it != server->getLocations().end()) {
		if (_requested_path == (*it)->getRoute()) {
			return *it;
		}
		++it;
	}
	return NULL;
}

void Parser::getParam(s_info_param &info, int param,
					  const statement::Location *location)
{
	if (param & F_ROOT) {
		info.root = location->getRoot();
	}
	if (param & F_INDEX) {
		std::cout << "index: " << location->getIndex() << std::endl;
		info.index = location->getIndex();
	}
	if (param & F_AUTOINDEX) {
		info.autoindex = location->getAutoindex();
	}
	if (param & F_RETURN) {
		statement::ParamDouble value = location->getReturns();
		info.return_code = value.getValue1();
		info.return_value = value.getValue();
	}
	if (param & F_ERRORPAGE) {
		const std::vector< const statement::ErrorPage * > errorPages =
			location->getErrorPages();

		std::vector< const statement::ErrorPage * >::const_iterator it_pages =
			errorPages.begin();

		while (it_pages != errorPages.end()) {
			std::vector< string >::const_iterator it_code =
				(*it_pages)->getErrorCode().begin();

			while (it_code != (*it_pages)->getErrorCode().end()) {
				if (_codeResponse == *it_code) {
					info.error_page = (*it_pages)->getValue();
					break;
				}
				++it_code;
			}
			++it_pages;
		}
	}
	if (param & F_DENYMETHOD) {
		info.deny_methods = location->getDenyMethods();
	}
}

void Parser::getParam(s_info_param &info, int param, const Server *server)
{
	if (param & F_ROOT) {
		info.root = server->getRoot();
	}
	if (param & F_INDEX) {
		info.index = server->getIndex();
	}
	if (param & F_AUTOINDEX) {
		info.autoindex = server->getAutoindex();
	}
	if (param & F_RETURN) {
		statement::ParamDouble value = server->getReturns();
		info.return_code = value.getValue1();
		info.return_value = value.getValue();
	}
	if (param & F_ERRORPAGE) {
		const std::vector< const statement::ErrorPage * > errorPages =
			server->getErrorPages();

		std::vector< const statement::ErrorPage * >::const_iterator it_pages =
			errorPages.begin();

		while (it_pages != errorPages.end()) {
			std::vector< string >::const_iterator it_code =
				(*it_pages)->getErrorCode().begin();

			while (it_code != (*it_pages)->getErrorCode().end()) {
				if (_codeResponse == *it_code) {
					info.error_page = (*it_pages)->getValue();
					break;
				}
				++it_code;
			}
			++it_pages;
		}
	}
	if (param & F_DENYMETHOD) {
		info.deny_methods = server->getDenyMethods();
	}
}

bool Parser::find_param_location(s_info_param &info, int param, int annexes)
{
	const statement::Location *location = find_location(_server);

	if (location == NULL) {
		return false;
	}
	getParam(info, param, location);
	if (info.isEmpty()) {
		return false;
	}
	if (annexes != 0) {
		getConfigParam(info, annexes, 0);
	}
	return true;
}

bool Parser::find_param_server(s_info_param &info, int param, int annexes)
{
	getParam(info, param, _server);
	if (info.isEmpty()) {
		return false;
	}
	if (annexes != 0) {
		getConfigParam(info, annexes, 0, 1);
	}
	return true;
}

bool Parser::find_param_location_default(s_info_param &info, int param,
										 int annexes)
{
	const statement::Location *location = find_location(_default_server);

	if (location == NULL) {
		return false;
	}
	getParam(info, param, location);
	if (info.isEmpty()) {
		return false;
	}
	if (annexes != 0) {
		getConfigParam(info, annexes, 0, 2);
	}
	return true;
}

bool Parser::find_param_server_default(s_info_param &info, int param,
									   int annexes)
{
	if (_default_server == NULL) {
		return false;
	}
	getParam(info, param, _default_server);
	if (info.isEmpty()) {
		return false;
	}
	if (annexes != 0) {
		getConfigParam(info, annexes, 0, 3);
	}
	return true;
}

// string Parser::find_hard_code(const string &path, IdentifyParam
// identifier)
// {
// 	{
// 		(void)path;
// 		(void)identifier;
// 		if (identifier == P_ERRORPAGE) {
// 			return " < head > < h1 > ERROR " + _codeResponse +
// 				   findStatusMessage(_codeResponse) +
// 				   "Sorry for this ugly page bro </h1></head>";
// 		}
// 		return "";
// 	}
// }