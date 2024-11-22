/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executeCGI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:03:31 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/22 14:31:15 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>

#include "cgi/CGIHandler.hpp"

using std::string;

int CGIHandler::childProcess()
{
	alarm(TIMEOUT);

	if (Logger::Instance) {
		Logger::Instance->resetBuffer();
	}

	if (chdir(_path.c_str()) != 0) {
		LOG_WARNING("Chdir Error: " + string(strerror(errno)));
		delete Logger::Instance;
		exit(FAILURE);
	}

	if (close(_pipeOut[READ]) != 0 || close(_pipeIn[WRITE]) != 0) {
		LOG_WARNING("Close Error: " + string(strerror(errno)));
		delete Logger::Instance;
		exit(FAILURE);
	}

	if (dup2(_pipeOut[WRITE], STDOUT_FILENO) == -1 ||
		dup2(_pipeIn[READ], STDIN_FILENO) == -1) {
		LOG_WARNING("Dup2 Error: " + string(strerror(errno)));
		delete Logger::Instance;
		exit(FAILURE);
	}

	if (close(_pipeOut[WRITE]) != 0 || close(_pipeIn[READ]) != 0) {
		LOG_WARNING("Close Error: " + string(strerror(errno)));
		delete Logger::Instance;
		exit(FAILURE);
	}

	delete Logger::Instance;

	if (execve(_cgi.c_str(), _argv.data(), _envp.data()) != 0) {
		exit(errno);
	}

	exit(EXIT_FAILURE);
}

int CGIHandler::parentProcess()
{
	if (close(_pipeOut[WRITE]) != 0 || close(_pipeIn[READ]) != 0) {
		LOG_WARNING("Close Error: " + string(strerror(errno)));
		return FAILURE;
	}
	if (write(_pipeIn[WRITE], _body.c_str(), _body.size()) == -1) {
		LOG_WARNING("Write Error: " + string(strerror(errno)));
		return FAILURE;
	}
	if (close(_pipeIn[WRITE]) != 0) {
		LOG_WARNING("Close Error: " + string(strerror(errno)));
		return FAILURE;
	}
	return SUCCESS;
}

int CGIHandler::recvCGIResponse(std::string &response)
{
	char	buffer[BUFFER_SIZE];
	ssize_t nb_byte = 1;

	while (nb_byte > 0) {
		memset(buffer, 0, BUFFER_SIZE);
		nb_byte = read(_pipeOut[READ], buffer, BUFFER_SIZE);
		if (nb_byte == -1) {
			LOG_WARNING("Read Error: " + string(strerror(errno)));
			return FAILURE;
		}
		if (nb_byte == 0) {
			break;
		}
		response.append(buffer, nb_byte);
	}
	return SUCCESS;
}

int CGIHandler::waitCGI()
{
	pid_t ret = 0;

	if (_status == CGI_FAIL) {
		return CGI_FAIL;
	}
	ret = waitpid(_pid, &_status, WNOHANG);
	if (ret < 0) {
		LOG_WARNING("Waitpid Error: " + string(strerror(errno)));
		return CGI_FAIL;
	}
	if (ret == CGI_WAIT) {
		return CGI_WAIT;
	}
	return CGI_DONE;
}

int CGIHandler::execute()
{
	if (pipe(_pipeIn) != 0 || pipe(_pipeOut) != 0) {
		LOG_WARNING("Pipe Error: " + string(strerror(errno)));
		return FAILURE;
	}
	_pid = fork();
	if (_pid < 0) {
		LOG_WARNING("Fork Error: " + string(strerror(errno)));
		return FAILURE;
	}
	if (_pid == 0) {
		if (childProcess() == FAILURE) {
			return FAILURE;
		}
	}
	if (parentProcess() == FAILURE) {
		return FAILURE;
	}
	return SUCCESS;
}
