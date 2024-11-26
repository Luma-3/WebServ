/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:59:40 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/18 14:30:32 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

#include <cerrno>
#include <cstring>
#include <ctime>

#include "server/VirtualServer.hpp"

Logger				*Logger::Instance = NULL;
const VirtualServer *current = NULL;

Logger::Logger(const std::string &filename, int logLevel) :
	_filename(filename),
	_buffer(),
	_buffer_size(0),
	_logLevel(logLevel)
{
	if (Instance) {
		throw std::runtime_error("Logger instance already exists");
	}
	Instance = this;

	const std::string new_filename = formatFile(filename);

	_file.open(new_filename.c_str(), std::ios::app);
	if (!_file.is_open()) {
		std::cerr << "Error: could not open file: " << strerror(errno) << " "
				  << new_filename << std::endl;
	}
}

std::string Logger::formatFile(const std::string &filename)
{
	const size_t last_dot = filename.find_last_of('.');
	size_t		 last_slash = filename.find_last_of('/');

	std::string extension =
		(last_dot == std::string::npos) ? "" : filename.substr(last_dot);

	extension = extension.empty() ? ".log" : extension;
	last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;

	const std::string path = filename.substr(0, last_slash);
	const std::string file_without_extension =
		filename.substr(last_slash, last_dot - last_slash);

	const std::string new_filename =
		path + "[" + formatTime() + "]" + file_without_extension + extension;

	return new_filename;
}

void Logger::resetBuffer()
{
	_buffer_size = 0;
	memset(_buffer, 0, BUFFER_SIZE);
}

Logger::~Logger()
{
	flush();
	_file.close();
	Instance = NULL;
}

Logger &Logger::operator=(const Logger &rhs)
{
	if (this != &rhs) {
		_buffer_size = rhs._buffer_size;
		_logLevel = rhs._logLevel;
		if (!_file.is_open()) {
			std::cerr << "Error: could not open file " << rhs._filename
					  << std::endl;
		}
		std::memcpy(_buffer, rhs._buffer, BUFFER_SIZE);
	}
	return *this;
}

std::string Logger::formatTime()
{
	const std::time_t time = std::time(0);
	std::tm			 *ltm = std::localtime(&time);
	char			  time_str[BUFFER_SIZE];

	strftime(time_str, BUFFER_SIZE, "%d-%m-%Y_%H:%M:%S", ltm);
	return std::string(time_str);
}

std::string Logger::formatServer()
{
	if (CSERVER) {

		const std::string hostname = CSERVER->getParamValue("hostname");

		if (hostname.empty()) {
			const std::string ip = CSERVER->getParamValue("listen");
			const std::string port = CSERVER->getParamValue("port");
			return ip + ":" + port;
		}
		return hostname;
	}
	return "Main Server";
}

// [INFO] 127.0.0.1:8080 -> message : 04-10-2024 14:33:51

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
		const std::string log_message = "[" + level_str + "] " +
										formatServer() + " -> " + message +
										" : " + formatTime() + "\n";
		const size_t message_size = log_message.size();

		if (_buffer_size + message_size >= BUFFER_SIZE) {
			flush();
		}

		std::memcpy(_buffer + _buffer_size, log_message.c_str(), message_size);
		_buffer_size += message_size;
	}
}

std::string Logger::removeColor(const std::string &str)
{
	std::string new_str = str;
	size_t		pos = 0;

	while ((pos = new_str.find("\033[")) != std::string::npos) {

		const size_t end = new_str.find('m', pos);
		if (end != std::string::npos) {
			new_str.erase(pos, end - pos + 1);
		}
	}
	return new_str;
}

LogLevel Logger::StringToLogLevel(const std::string &str)
{
	if (str == "Debug") {
		return DEBUG;
	}
	if (str == "Info") {
		return INFO;
	}
	if (str == "Warning") {
		return WARNING;
	}
	if (str == "Error") {
		return ERROR;
	}
	return DEBUG;
}

void Logger::flush()
{

	if (_buffer_size > 0) {
		_file.write(_buffer, static_cast< std::streamsize >(_buffer_size));
		_file.flush();
		_buffer_size = 0;
		memset(_buffer, 0, BUFFER_SIZE);
	}
}
