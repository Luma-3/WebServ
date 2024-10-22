/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executeCGI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:03:31 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/23 01:43:56 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

using std::string;

int CGIHandler::childProcess()
{
	alarm(TIMEOUT);

	close(_pipefd[READ]);
	dup2(_pipefd[WRITE], STDOUT_FILENO);
	close(_pipefd[WRITE]);
	if (execve(_cgi, _argv, _envp) == -1) {
		throw std::runtime_error("Execve Error: " + string(strerror(errno)));
	}
	return 0;
}

int CGIHandler::parentProcess()
{
	close(_pipefd[WRITE]);
	return 0;
}

void CGIHandler::recvCGIResponse()
{
	char	buffer[1024];
	ssize_t nb_byte = 1;

	while (nb_byte > 0) {
		memset(buffer, 0, 1024);
		nb_byte = read(_pipefd[READ], buffer, 1024);
		std::cout << "Buffer: " << buffer << std::endl;
		if (nb_byte == -1) {
			throw std::runtime_error("Read Error: " + string(strerror(errno)));
		}
		if (nb_byte == 0) {
			break;
		}
		_response.append(buffer, nb_byte);
	}
	std::cout << "RECIEVED: " << _response << std::endl;
}

int CGIHandler::waitCGI()
{
	pid_t ret = 0;
	ret = waitpid(_pid, &_status, WNOHANG);
	if (ret < 0) {
		throw std::runtime_error("Waitpid Error: " + string(strerror(errno)));
	}
	else if (ret == 0) {
		return 0;
	}
	else {
		if (WIFEXITED(_status) && WEXITSTATUS(_status) != 0) {
			std::cerr << "CGI Error: " << WEXITSTATUS(_status)
					  << std::endl; // TODO: Log this and return 500
		}
		return 1;
	}
}

void CGIHandler::execute()
{
	if (pipe(_pipefd) == -1) {
		throw std::runtime_error("Pipe Error: " + string(strerror(errno)));
	}
	_pid = fork();
	if (_pid < 0) {
		throw std::runtime_error("Fork Error: " + string(strerror(errno)));
	}
	else if (_pid == 0) {
		childProcess();
	}
	else {
		parentProcess();
	}
}
