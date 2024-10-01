/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_Tables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:39:08 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/01 10:05:44 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

using std::map;
using std::string;

string client::Builder::findContentType(const string &file_extension)
{
	static map< string, string > content_type_list;
	content_type_list["html"] = "text/html";
	content_type_list["css"] = "text/css";
	content_type_list["js"] = "text/javascript";
	content_type_list["jpg"] = "image/jpeg";
	content_type_list["jpeg"] = "image/jpeg";
	content_type_list["png"] = "image/png";
	content_type_list["gif"] = "image/gif";
	content_type_list["bmp"] = "image/bmp";
	content_type_list["ico"] = "image/x-icon";
	content_type_list["svg"] = "image/svg+xml";
	content_type_list["json"] = "application/json";
	content_type_list["pdf"] = "application/pdf";
	content_type_list["zip"] = "application/zip";
	content_type_list["tar"] = "application/x-tar";

	if (content_type_list[file_extension].empty()) {
		return "text/plain";
	}
	return content_type_list[file_extension];
}

string client::Builder::findStatusMessage(const std::string &code)
{
	static map< std::string, std::string > status_message;
	status_message["200"] = " OK";
	status_message["400"] = " Bad Request";
	status_message["403"] = " Forbidden";
	status_message["404"] = " Not Found";
	status_message["405"] = " Method Not Allowed";
	status_message["500"] = " Internal Server Error";

	if (status_message[code].empty()) {
		return " Unknown";
	}
	return status_message[code];
}