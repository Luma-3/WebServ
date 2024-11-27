/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:32:28 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/27 11:07:09 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <queue>
#include <string>

#include "Token.hpp"

#define MAXHOSTNAMESIZE 255
#define MAXPORTSIZE 5
#define MAXUCHAR 255

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
	~Lexer();
	Lexer(const char *file_path);

	std::queue< Token * >		&getTokens() { return _tokens; }
	const std::queue< Token * > &getTokens() const { return _tokens; };
	const std::ifstream			&getConfigFile() const { return _config_file; };

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
