/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executeCGI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:03:31 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/04 09:45:11 by jbrousse         ###   ########.fr       */
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

	if (close(_pipeOut[READ]) != 0 || close(_pipeIn[WRITE]) != 0) {
		LOG_WARNING("Close Error: " + string(strerror(errno)), NULL);
		exit(FAILURE);
	}

	if (dup2(_pipeOut[WRITE], STDOUT_FILENO) == -1 ||
		dup2(_pipeIn[READ], STDIN_FILENO) == -1) {
		LOG_WARNING("Dup2 Error: " + string(strerror(errno)), NULL);
		exit(FAILURE);
	}

	if (close(_pipeOut[WRITE]) != 0 || close(_pipeIn[READ]) != 0) {
		LOG_WARNING("Close Error: " + string(strerror(errno)), NULL);
		exit(FAILURE);
	}

	if (Logger::Instance) {
		delete Logger::Instance;
	}

	if (execve(_cgi, _argv.data(), _envp.data()) != 0) {
		exit(errno);
	}

	exit(EXIT_FAILURE);
}

int CGIHandler::parentProcess()
{
	if (close(_pipeOut[WRITE]) != 0 || close(_pipeIn[READ]) != 0) {
		LOG_WARNING("Close Error: " + string(strerror(errno)), NULL);
		return FAILURE;
	}
	if (write(_pipeIn[WRITE], _body.c_str(), _body.size()) != 0) {
		LOG_WARNING("Write Error: " + string(strerror(errno)), NULL);
		return FAILURE;
	}
	if (close(_pipeIn[WRITE]) != 0) {
		LOG_WARNING("Close Error: " + string(strerror(errno)), NULL);
		return FAILURE;
	}
	return SUCCESS;
}

int CGIHandler::recvCGIResponse()
{
	char	buffer[1024];
	ssize_t nb_byte = 1;

	while (nb_byte > 0) {
		memset(buffer, 0, 1024);
		nb_byte = read(_pipeOut[READ], buffer, 1024);
		if (nb_byte == -1) {
			LOG_WARNING("Read Error: " + string(strerror(errno)), _CSERVER);
			return FAILURE;
		}
		if (nb_byte == 0) {
			break;
		}
		_response.append(buffer, nb_byte);
	}
	return SUCCESS;
}

int CGIHandler::waitCGI()
{
	pid_t ret = 0;

	if (_status == CGI_FAIL) {
		std::cerr << "CGI Fail with status: " << _status << std::endl;
		return CGI_FAIL;
	}
	ret = waitpid(_pid, &_status, WNOHANG);
	if (ret < 0) {
		LOG_WARNING("Waitpid Error: " + string(strerror(errno)), _CSERVER);
		return CGI_FAIL;
	}
	else if (ret == CGI_WAIT) {
		return CGI_WAIT;
	}
	return CGI_DONE;
}

int CGIHandler::execute()
{
	if (pipe(_pipeIn) != 0 || pipe(_pipeOut) != 0) {
		LOG_WARNING("Pipe Error: " + string(strerror(errno)), _CSERVER);
		return FAILURE;
	}
	_pid = fork();
	if (_pid < 0) {
		LOG_WARNING("Fork Error: " + string(strerror(errno)), _CSERVER);
		return FAILURE;
	}
	else if (_pid == 0) {
		if (childProcess() == FAILURE) {
			return FAILURE;
		}
	}
	else {
		if (parentProcess() == FAILURE) {
			return FAILURE;
		}
	}
	return SUCCESS;
}
