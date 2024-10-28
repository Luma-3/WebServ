/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:44:22 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/28 14:40:43 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>

class VirtualServer;

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

#define LOG_DEBUG(message, server)                      \
	if (Logger::Instance) {                             \
		Logger::Instance->log(DEBUG, message, server);  \
	}                                                   \
	else {                                              \
		std::cerr << "No logger instance" << std::endl; \
		std::cerr << message << std::endl;              \
	}

#define LOG_INFO(message, server)                       \
	if (Logger::Instance) {                             \
		Logger::Instance->log(INFO, message, server);   \
	}                                                   \
	else {                                              \
		std::cerr << "No logger instance" << std::endl; \
		std::cerr << message << std::endl;              \
	}

#define LOG_WARNING(message, server)                     \
	if (Logger::Instance) {                              \
		Logger::Instance->log(WARNING, message, server); \
	}                                                    \
	else {                                               \
		std::cerr << "No logger instance" << std::endl;  \
		std::cerr << message << std::endl;               \
	}

#define LOG_ERROR(message, server)                      \
	if (Logger::Instance) {                             \
		Logger::Instance->log(ERROR, message, server);  \
	}                                                   \
	else {                                              \
		std::cerr << "No logger instance" << std::endl; \
		std::cerr << message << std::endl;              \
	}

class Logger
{
  private:
	std::ofstream _file;
	char		  _buffer[BUFFER_SIZE];
	size_t		  _buffer_size;
	int			  _logLevel;

	static std::string formatFile(const std::string &filename);
	static std::string formatTime();
	static std::string formatServer(const VirtualServer *server);

  public:
	static Logger *Instance;

	Logger(const std::string &filename, int logLevel);
	~Logger();

	Logger &operator=(const Logger &rhs);

	void log(LogLevel level, const std::string &message,
			 const VirtualServer *server = NULL);

	static LogLevel StringToLogLevel(const std::string &str);

	void resetBuffer();

	void flush();
};

#endif // LOGGER_HPP