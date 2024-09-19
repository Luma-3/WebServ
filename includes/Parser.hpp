/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:13:07 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/19 22:06:23 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

using std::map;
using std::string;


#define DEFAULT_ERROR_STYLE "default_page/css/style.css"
#define DEFAULT_ERROR_IMG	"default_page/images/bg.jpg"

namespace client {

class Parser
{
  private:
	map< string, string > _headers;
	string				  _buffer;
	bool				  _haveHeader;
	string				  _url_path;
	string				  _filename;
	string				  _file_extension;
	string				  _codeResponse;

	void		  changeUrlError();
	bool		  InvalidMethod();
	bool		  InvalidHeader();
	void		  checkRequest();
	void		  getBodyFromRequest(size_t &line_break_pos);
	static string getAndErase(string &str, const string &delim);
	void		  handleUrl(std::string &url);
	void		  getHeaderFromRequest(const size_t &line_break_pos);
	string		  extractPathAndFilename(string &url, const size_t &l_slash,
										 const size_t &l_dot);
	string		  extractExtension(string &url, const size_t &l_dot);

  public:
	Parser();
	~Parser();
	void				   parseRequest(void *buff);
	map< string, string > &getHeaders();
	string				  &getUrlPath();
	string				  &getFilename();
	string				  &getFileExtension();
	string				  &getCodeResponse();
};

} // namespace client

#endif
