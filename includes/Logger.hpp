/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:44:22 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/07 18:12:58 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>

#define BUFFER_SIZE 1024

enum LogLevel {
	DEBUG = 0,
	INFO,
	WARNING,
	ERROR
};

class Logger
{
  private:
	std::ofstream _file;
	char		  _buffer[BUFFER_SIZE];
	size_t		  _buffer_size;
	int			  _logLevel;

  public:
	Logger();
	Logger(const std::string &filename, int logLevel);
	Logger(const Logger &src);
	~Logger();

	Logger &operator=(const Logger &rhs);

	void log(LogLevel level, const std::string &message);

	static LogLevel StringToLogLevel(const std::string &str);

	void flush();
};

#endif // LOGGER_HPP