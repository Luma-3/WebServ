/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:13:07 by Monsieur_Ca       #+#    #+#             */
/*   Updated: 2024/09/19 14:02:52 by Monsieur_Ca      ###   ########.fr       */
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

#include "Client.hpp"

using std::map;
using std::string;

namespace client {

class Parser
{
  private:
	map< string, string > _headers;
	string				  _buffer;
	bool				  _haveHeader;
	string				  _codeResponse;

	static string findContentType(const std::string &file_extension);
	void		  changeUrlError();
	bool		  InvalidMethod();
	bool		  InvalidHeader();
	bool		  checkRequest();
	void		  getBodyFromRequest(size_t &line_break_pos);
	static string getAndErase(string &str, const string &delim);
	void		  createUrl(std::string &url);
	static void	  setDefaultUrl(string &url, string &filename,
								string &file_extension);
	void		  getHeaderFromRequest(const size_t &line_break_pos);

  public:
	Parser();
	~Parser();
	void parseRequest(void *buff);
};

} // namespace client

#endif
