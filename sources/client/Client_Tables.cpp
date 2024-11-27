/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_Tables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:39:08 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/11/27 11:06:08 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Builder.hpp"

using std::map;
using std::string;

string findContentType(const string &file_extension)
{
	static map< string, string > content_type_list;
	content_type_list["html"] = "text/html";
	content_type_list["css"] = "text/css";
	content_type_list["js"] = "application/javascript";
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
		return "application/octet-stream";
	}
	return content_type_list[file_extension];
}

string findStatusMessage(const std::string &code)
{
	static map< std::string, std::string > status_message;
	status_message["200"] = " OK";
	status_message["201"] = " Created";
	status_message["204"] = " No Content";
	status_message["301"] = " Moved Permanently";
	status_message["302"] = " Found";
	status_message["303"] = " See Other";
	status_message["304"] = "Not Modified";
	status_message["307"] = " Temporary Redirect";
	status_message["308"] = " Permanent Redirect";
	status_message["400"] = " Bad Request";
	status_message["401"] = " Unauthorized";
	status_message["402"] = " Payment Required";
	status_message["403"] = " Forbidden";
	status_message["404"] = " Not Found";
	status_message["405"] = " Method Not Allowed";
	status_message["406"] = " Not Acceptable";
	status_message["407"] = " Proxy Authentication Required";
	status_message["408"] = " Request Timeout";
	status_message["409"] = " Conflict";
	status_message["410"] = " Gone";
	status_message["411"] = " Length Required";
	status_message["412"] = " Precondition Failed";
	status_message["413"] = " Request Entity Too Large";
	status_message["418"] = " I'm a teapot";
	status_message["500"] = " Internal Server Error";
	status_message["501"] = " Not Implemented";
	status_message["502"] = " Bad Gateway";
	status_message["503"] = " Service Unavailable";
	status_message["504"] = " Gateway Timeout";
	status_message["505"] = " HTTP Version Not Supported";

	if (status_message[code].empty()) {
		return " Unknown";
	}
	return status_message[code];
}
