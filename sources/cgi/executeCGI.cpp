/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executeCGI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:03:31 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/21 15:04:14 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGIHandler.hpp"

using std::string;

namespace {

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
	int status;

	close(_pipefd[WRITE]);
	dup2(_pipefd[READ], STDIN_FILENO);
	close(_pipefd[READ]);
	recvCGIResponse(STDIN_FILENO, _response);
	waitpid(_pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGALRM) {
		std::cerr << "CGI Timeout"
				  << std::endl; // TODO: Log this and return 500
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		std::cerr << "CGI Error: " << WEXITSTATUS(status)
				  << std::endl; // TODO: Log this and return 500
	}
	adjustHeader(_response);
	return 0;
}
} // namespace

void recvCGIResponse(int fd, std::string *response)
{
	char buffer[1024];
	int	 ret;

	while ((ret = read(fd, buffer, 1024)) > 0) {
		response->append(buffer, ret);
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
