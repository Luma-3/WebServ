/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:59:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/07 19:02:40 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

#include <cerrno>
#include <cstring>

Logger::Logger() : _buffer_size(0), _logLevel(DEBUG) {}

Logger::Logger(const std::string &filename, int logLevel) :
	_file(filename.c_str(), std::ios::app),
	_buffer_size(0),
	_logLevel(logLevel)
{
	std::cout << "Opening file: " << filename << std::endl;
	if (!_file.is_open()) {
		std::cerr << "Error: could not open file: " << strerror(errno) << " "
				  << filename << std::endl;
	}
}

Logger::Logger(const Logger &src) :
	_buffer_size(src._buffer_size),
	_logLevel(src._logLevel)
{
	if (!_file.is_open()) {
		std::cerr << "Error: could not open file " << src._file << std::endl;
	}
	std::memcpy(_buffer, src._buffer, BUFFER_SIZE);
}

Logger::~Logger()
{
	flush();
	_file.close();
}

Logger &Logger::operator=(const Logger &rhs)
{
	if (this != &rhs) {
		// _file = rhs._file;
		_buffer_size = rhs._buffer_size;
		_logLevel = rhs._logLevel;
		if (!_file.is_open()) {
			std::cerr << "Error: could not open file " << rhs._file
					  << std::endl;
		}
		std::memcpy(_buffer, rhs._buffer, BUFFER_SIZE);
	}
	return *this;
}

void Logger::log(LogLevel level, const std::string &message)
{
	if (level >= _logLevel) {
		std::string level_str;
		switch (level) {
			case DEBUG:
				level_str = "DEBUG";
				break;
			case INFO:
				level_str = "INFO";
				break;
			case WARNING:
				level_str = "WARNING";
				break;
			case ERROR:
				level_str = "ERROR";
				break;
			default:
				level_str = "UNKNOWN";
				break;
		}
		std::string log_message = "[" + level_str + "] " + message + "\n";
		size_t		message_size = log_message.size();
		if (_buffer_size + message_size >= BUFFER_SIZE) {
			flush();
		}
		std::memcpy(_buffer + _buffer_size, log_message.c_str(), message_size);
		_buffer_size += message_size;
	}
}

LogLevel Logger::StringToLogLevel(const std::string &str)
{
	if (str == "Debug") {
		return DEBUG;
	}
	else if (str == "Info") {
		return INFO;
	}
	else if (str == "Warning") {
		return WARNING;
	}
	else if (str == "Error") {
		return ERROR;
	}
	else {
		return DEBUG;
	}
}

void Logger::flush()
{
	if (_buffer_size > 0) {
		_file.write(_buffer, _buffer_size);
		_file.flush();
		_buffer_size = 0;
	}
}