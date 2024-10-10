/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:13:07 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/10 15:09:50 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_PARSER_HPP
#define C_PARSER_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>

#include "server/Server.hpp"
#include "template/StringUtils.tpp"

namespace client {

enum IdentifyParam {
	P_INDEX,
	P_ROOT,
	P_AUTOINDEX,
	P_RETURN,
	P_ERRORPAGE,
	P_DENYMETHOD,
};
class Parser
{
  private:
	const Server *_server;
	const Server *_default_server;

	std::map< std::string, std::string > _headers;
	std::vector< std::string >			 _methods;

	std::string _buffer;
	std::string _requested_path;
	std::string _path;
	std::string _filename;
	std::string _codeResponse;

	void getHeaderFromRequest(const size_t &line_break_pos);
	void getBodyFromRequest(size_t &line_break_pos);

	void handleRequestedPath(std::string &url);

	std::string getConfigParam(
		const std::string									&param,
		std::vector< std::string (*)(const std::string &) > &functions);

	const statement::Location *find_location(const std::string &path,
											 const Server	   *server);
	std::string				   find_param_location(const std::string &path,
												   IdentifyParam	  identifier);
	std::string				   find_param_server(const std::string &path,
												 IdentifyParam		identifier);
	std::string find_param_location_default(const std::string &path,
											IdentifyParam	   identifier);
	std::string find_param_default_server(const std::string &path,
										  IdentifyParam		 identifier);
	std::string find_hard_code(const std::string &path,
							   IdentifyParam	  identifier);

	std::string getParam(IdentifyParam				identifier,
						 const statement::Location *location);
	std::string getParam(IdentifyParam identifier, const Server *Server);

	bool InvalidMethod();
	bool InvalidHeader();

  public:
	Parser();
	Parser(const Parser &src);
	Parser(const Server *server, const Server *default_server);
	Parser &operator=(const Parser &src);
	~Parser();

	void parseRequest(const std::string &request);

	void reset();

	void setPath(const std::string &path);
	void setCodeResponse(const std::string &code);
	void setFilename(const std::string &filename);
	void setPathAndFilename(const std::string &path,
							const std::string &filename);

	const std::map< std::string, std::string > &getHeaders() const;
	const std::string						   &getRequestedPath() const;
	const std::string						   &getPath() const;
	const std::string						   &getFilename() const;
	const std::string							getFileExtension() const;
	const std::string						   &getCodeResponse() const;
	std::pair< std::string, std::string >
	getConfigParam(const std::string &param, IdentifyParam identify);
};

} // namespace client

#endif