/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:13:07 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/10/01 10:20:20 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_PARSER_HPP
#define C_PARSER_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>

#include "Parser_utils.tpp"

namespace client {

class Parser
{
  private:
	std::map< std::string, std::string > _headers;
	std::string							 _buffer;
	std::string							 _requested_path;
	std::string							 _filename;
	std::string							 _file_extension;
	std::string							 _codeResponse;

	void getHeaderFromRequest(const size_t &line_break_pos);
	void getBodyFromRequest(size_t &line_break_pos);

	static std::string extractExtension(std::string &url, const size_t &l_dot);
	void			   handleUrl(std::string &url);
	std::string extractPathAndFilename(std::string &url, const size_t &l_slash,
									   const size_t &l_dot);

	bool InvalidMethod();
	bool InvalidHeader();

  public:
	Parser();
	Parser(const Parser &src);
	Parser &operator=(const Parser &src);
	~Parser();

	void parseRequest(const std::string &request);

	void reset();

	const std::map< std::string, std::string > &getHeaders() const;
	const std::string						   &getRequestedPath() const;
	const std::string						   &getFilename() const;
	const std::string						   &getFileExtension() const;
	const std::string						   &getCodeResponse() const;
};

} // namespace client

#endif