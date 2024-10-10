/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finderConfigParam.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:50:17 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/10 14:20:17 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"
#include "client/Parser.hpp"

using client::IdentifyParam;
using client::Parser;
using std::string;

const statement::Location *Parser::find_location(const string &path,
												 const Server *server)
{
	std::vector< const statement::Location * >::const_iterator it =
		server->getLocations().begin();

	while (it != server->getLocations().end()) {
		if (path == (*it)->getRoute()) {
			return *it;
		}
		++it;
	}
	return NULL;
}

string Parser::getParam(IdentifyParam			   identifier,
						const statement::Location *location)
{
	switch (identifier) {
		case P_ROOT:
			return location->getRoot();
		case P_INDEX:
			return location->getIndex();
		case P_AUTOINDEX: {
			bool state = location->getAutoindex();
			return state ? "on" : "off";
		}
		case P_RETURN: {
			statement::ParamDouble value = location->getReturn();
			_codeResponse = value.getValue1();
			return value.getValue();
		}
		case P_ERRORPAGE: {
			const std::vector< const statement::ErrorPage * > errorPages =
				location->getErrorPages();
			std::vector< const statement::ErrorPage * >::const_iterator
				it_pages = errorPages.begin();

			while (it_pages != errorPages.end()) {
				std::vector< string >::const_iterator it_code =
					(*it_pages)->getErrorCode().begin();

				while (it_code != (*it_pages)->getErrorCode().end()) {
					if (_codeResponse == *it_code) {
						return location->getRoot() + (*it_pages)->getValue();
					}
					++it_code;
				}
				++it_pages;
			}
			break;
		}
		case P_DENYMETHOD: {
			_methods = location->getDenyMethods();
			if (_methods.empty()) {
				return "";
			}
			return "true";
		}
		default:
			break;
	}
	return "";
}

string Parser::getParam(IdentifyParam identifier, const Server *server)
{
	switch (identifier) {
		case P_ROOT:
			return server->getRoot();
		case P_INDEX:
			return server->getIndex();
		case P_AUTOINDEX: {
			bool state = server->getAutoindex();
			return state ? "on" : "off";
		}
		case P_RETURN: {
			statement::ParamDouble value = server->getReturns();
			_codeResponse = value.getValue1();
			return value.getValue();
		}
		case P_ERRORPAGE: {
			const std::vector< const statement::ErrorPage * > errorPages =
				server->getErrorPages();
			std::vector< const statement::ErrorPage * >::const_iterator
				it_pages = errorPages.begin();

			while (it_pages != errorPages.end()) {

				std::vector< string >::const_iterator it_code =
					(*it_pages)->getErrorCode().begin();

				while (it_code != (*it_pages)->getErrorCode().end()) {
					if (_codeResponse == *it_code) {
						return (*it_pages)->getValue();
					}
					++it_code;
				}
				++it_pages;
			}
			break;
		}
		case P_DENYMETHOD: {
			_methods = server->getDenyMethods();
			if (_methods.empty()) {
				return "";
			}
			return "true";
		}
		default:
			break;
	}
	return "";
}

string Parser::find_param_location(const string &path, IdentifyParam identifier)
{
	const statement::Location *location = find_location(path, _server);

	if (location == NULL) {
		return "";
	}
	return getParam(identifier, location);
}

string Parser::find_param_server(const string &path, IdentifyParam identifier)
{
	{
		(void)path;
		return getParam(identifier, _server);
	}
}

string Parser::find_param_location_default(const string &path,
										   IdentifyParam identifier)
{
	{
		if (_default_server == NULL) {
			return "";
		}
		const statement::Location *location =
			find_location(path, _default_server);

		if (location == NULL) {
			return "";
		}
		return getParam(identifier, location);
	}
}

string Parser::find_param_default_server(const string &path,
										 IdentifyParam identifier)
{
	if (_default_server == NULL) {
		return "";
	}
	(void)path;
	return getParam(identifier, _default_server);
}

// string Parser::find_hard_code(const string &path, IdentifyParam identifier)
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