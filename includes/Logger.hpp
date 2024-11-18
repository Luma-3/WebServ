/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:44:22 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/18 12:55:26 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>

#include "color.hpp"

class VirtualServer;

extern const VirtualServer *current;

#define CSERVER current

#define BUFFER_SIZE 1024

enum LogLevel {
	DEBUG = 0,
#define DEBUG DEBUG
	INFO,
#define INFO INFO
	WARNING,
#define WARNING WARNING
	ERROR
#define ERROR ERROR
};

#define LOG_DEBUG(message)                                                    \
	if (Logger::Instance) {                                                   \
		Logger::Instance->log(DEBUG, Logger::removeColor(message));           \
	}                                                                         \
	else {                                                                    \
		std::cerr << PASTEL_RED "No logger instance"                          \
				  << RESET " (to have one, put this line in the confi_file: " \
						   "log_file [Log_Level] 'path')"                     \
				  << std::endl;                                               \
		std::cerr << (message) << std::endl;                                  \
	}

#define LOG_INFO(message)                                                     \
	if (Logger::Instance) {                                                   \
		Logger::Instance->log(INFO, Logger::removeColor(message));            \
	}                                                                         \
	else {                                                                    \
		std::cerr << PASTEL_RED "No logger instance"                          \
				  << RESET " (to have one, put this line in the confi_file: " \
						   "log_file [Log_Level] 'path')"                     \
				  << std::endl;                                               \
		std::cerr << (message) << std::endl;                                  \
	}

#define LOG_WARNING(message)                                                  \
	if (Logger::Instance) {                                                   \
		Logger::Instance->log(WARNING, Logger::removeColor(message));         \
	}                                                                         \
	else {                                                                    \
		std::cerr << PASTEL_RED "No logger instance"                          \
				  << RESET " (to have one, put this line in the confi_file: " \
						   "log_file [Log_Level] 'path')"                     \
				  << std::endl;                                               \
		std::cerr << (message) << std::endl;                                  \
	}

#define LOG_ERROR(message)                                                    \
	if (Logger::Instance) {                                                   \
		Logger::Instance->log(ERROR, Logger::removeColor(message));           \
	}                                                                         \
	else {                                                                    \
		std::cerr << PASTEL_RED "No logger instance"                          \
				  << RESET " (to have one, put this line in the confi_file: " \
						   "log_file [Log_Level] 'path')"                     \
				  << std::endl;                                               \
		std::cerr << (message) << std::endl;                                  \
	}

class Logger
{
  private:
	std::string	  _filename;
	std::ofstream _file;
	char		  _buffer[BUFFER_SIZE];
	size_t		  _buffer_size;
	int			  _logLevel;

	static std::string formatFile(const std::string &filename);
	static std::string formatTime();
	static std::string formatServer();

  public:
	static Logger *Instance;

	Logger(const std::string &filename, int logLevel);
	~Logger();

	Logger &operator=(const Logger &rhs);

	void log(LogLevel level, const std::string &message);

	static LogLevel StringToLogLevel(const std::string &str);

	void resetBuffer();

	static std::string removeColor(const std::string &str);

	void flush();
};

#endif // LOGGER_HPP