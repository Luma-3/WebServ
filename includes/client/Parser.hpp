/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:13:07 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/24 15:17:48 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <sstream>
#include <string>
#include <unistd.h>

using std::map;
using std::string;

#include "Parser_utils.tpp"

namespace client {

class Parser
{
  private:
	map< string, string > _headers;
	string				  _buffer;
	string				  _url_path;
	string				  _filename;
	string				  _file_extension;
	string				  _codeResponse;
	bool				  _haveHeader;

	void getHeaderFromRequest(const size_t &line_break_pos);
	void getBodyFromRequest(size_t &line_break_pos);

	static string extractExtension(string &url, const size_t &l_dot);
	void   handleUrl(std::string &url);
	string extractPathAndFilename(string &url, const size_t &l_slash,
								  const size_t &l_dot);

	bool InvalidMethod();
	bool InvalidHeader();

  public:
	Parser();
	Parser(const Parser &src);
	Parser &operator=(const Parser &src);
	~Parser();

	void parseRequest(void *buff);

	map< string, string > &getHeaders();
	const string		  &getUrlPath();
	const string		  &getFilename();
	const string		  &getFileExtension();
	const string		  &getCodeResponse();
};

} // namespace client

#endif
