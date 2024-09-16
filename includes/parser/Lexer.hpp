/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:32:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/16 14:52:22 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <string>
#include <vector>

#include "Token.hpp"

class Lexer
{
  private:
	std::ifstream		   _config_file;
	std::vector< Token * > _tokens;

	static bool IsDelimiter(char c);
	static void SkipSpace(std::string &line, size_t &it);
	static void TokenizeLine(std::string &line, std::vector< Token * > &tokens);

	static Token *CreateToken(size_t frontIT, size_t backIT, std::string &line);

  public:
	Lexer();
	~Lexer();
	Lexer(const Lexer &other);
	Lexer(const char *file_path);
	Lexer &operator=(const Lexer &other);

	const std::vector< Token * > &getTokens() const;
	const std::ifstream			 &getConfigFile() const;

	void Tokenize();

	// EXCEPTIONS

	class FileNotOpenException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		FileNotOpenException(std::string file_path);
		virtual const char *what() const throw();
		virtual ~FileNotOpenException() throw();
	};

	class InvalidFileExtensionException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		InvalidFileExtensionException(std::string extention);
		virtual const char *what() const throw();
		virtual ~InvalidFileExtensionException() throw();
	};
};

#endif // LEXER_HPP