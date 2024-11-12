/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:13:07 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/11/08 14:36:09 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_PARSER_HPP
#define C_PARSER_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>

#include "server/VirtualServer.hpp"
#include "template/StringUtils.tpp"

#define F_ROOT		  0x1
#define F_INDEX		  0x2
#define F_AUTOINDEX	  0x4
#define F_RETURN	  0x8
#define F_ERRORPAGE	  0x10
#define F_MAXBODYSIZE 0x20
#define F_DENYMETHOD  0x40

namespace client {

class Parser
{
  private:
	std::map< std::string, std::string > _headers;

	std::string _method;
	std::string _extension;
	std::string _buffer;
	std::string _requested_path;
	std::string _filename;
	std::string _codeResponse;
	std::string _path_info;
	std::string _query;

	void getHeaderFromRequest(const size_t &line_break_pos);
	void getBodyFromRequest(size_t &line_break_pos);

	void handleRequestedPath(std::string &requested_path);

	bool InvalidMethod();
	bool InvalidHeader();

  public:
	Parser();
	~Parser();

	static std::string findHostName(const std::string &request);

	void parseRequest(const std::string &request);

	const std::string &getRequestedPath() const { return _requested_path; };
	const std::string &getFilename() const { return _filename; };
	const std::string &getCodeResponse() const { return _codeResponse; };
	const std::string &getFileExtension() const { return _extension; };
	const std::string &getPathInfo() const { return _path_info; };
	std::string		   getHeader(const std::string &key) const;
	const std::string &getQuery() const { return _query; };
	void			   setFileExtension(std::string &extension)
	{
		_extension = extension;
	};

	static std::string findExtension(const std::string &filename);
};

} // namespace client

#endif