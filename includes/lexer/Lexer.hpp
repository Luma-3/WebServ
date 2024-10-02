/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:32:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/02 09:43:07 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <queue>
#include <string>
#include <vector>

#include "Token.hpp"

class Lexer
{
  private:
	std::ifstream		  _config_file;
	std::queue< Token * > _tokens;

	size_t _line;
	size_t _col;

	static bool IsDelimiter(char c);
	static void SkipSpace(const std::string &line, size_t &it);
	void TokenizeLine(const std::string &line, std::queue< Token * > &tokens);

	Token *CreateToken(size_t frontIT, size_t backIT,
					   const std::string &line) const;

  public:
	Lexer();
	~Lexer();
	Lexer(const Lexer &other);
	Lexer(const char *file_path);
	Lexer &operator=(const Lexer &other);

	std::queue< Token * >		&getTokens();
	const std::queue< Token * > &getTokens() const;
	const std::ifstream			&getConfigFile() const;

	void Tokenize();

	// EXCEPTIONS

	class InvalidFileExtensionException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		InvalidFileExtensionException(const std::string &extention);
		InvalidFileExtensionException(const InvalidFileExtensionException &src);

		InvalidFileExtensionException &
		operator=(const InvalidFileExtensionException &src);

		virtual ~InvalidFileExtensionException() throw();

		virtual const char *what() const throw();
	};

	class FileNotOpenException : public std::exception
	{
	  private:
		std::string _msg;

	  public:
		FileNotOpenException(const std::string &file_path);
		FileNotOpenException(const FileNotOpenException &src);

		FileNotOpenException &operator=(const FileNotOpenException &src);

		virtual ~FileNotOpenException() throw();

		virtual const char *what() const throw();
	};
};

#endif // LEXER_HPP