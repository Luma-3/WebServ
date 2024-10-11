/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:13:07 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/11 14:30:52 by jbrousse         ###   ########.fr       */
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

#define F_ROOT		  0x1
#define F_INDEX		  0x2
#define F_AUTOINDEX	  0x4
#define F_RETURN	  0x8
#define F_ERRORPAGE	  0x10
#define F_MAXBODYSIZE 0x20
#define F_DENYMETHOD  0x40

namespace client {

enum IdentifyParam {
	P_INDEX,
	P_ROOT,
	P_AUTOINDEX,
	P_RETURN,
	P_ERRORPAGE,
	P_DENYMETHOD,
};

typedef struct s_info_param {
	std::string				   root;
	std::string				   index;
	std::string				   autoindex;
	std::string				   return_code;
	std::string				   return_value;
	std::string				   error_page;
	std::string				   max_body_size;
	std::vector< std::string > deny_methods;

	bool isEmpty() const
	{
		if (root.empty() && index.empty() && autoindex.empty() &&
			return_code.empty() && return_value.empty() && error_page.empty() &&
			max_body_size.empty() && deny_methods.empty()) {
			return true;
		}
		return false;
	};
} t_info_param;

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

	const statement::Location *find_location(const Server *server);
	void					   getParam(s_info_param &info, int param,
										const statement::Location *location);
	void getParam(s_info_param &info, int param, const Server *server);
	bool find_param_location(s_info_param &info, int param, int annexes);
	bool find_param_server(s_info_param &info, int param, int annexes);
	bool find_param_location_default(s_info_param &info, int param,
									 int annexes);
	bool find_param_server_default(s_info_param &info, int param, int annexes);

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
	bool getConfigParam(s_info_param &info, int flags, int annexes,
						int start = 0);
};

} // namespace client

#endif