/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executeCGI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:03:31 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/16 16:26:13 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/CGI.hpp"

using std::string;

namespace {

int childProcess(exec_data *info)
{
	close(info->pipefd[READ]);
	dup2(info->pipefd[WRITE], STDOUT_FILENO);
	close(info->pipefd[WRITE]);
	if (execve(info->cgi, info->argv, info->envp) == -1) {
		throw std::runtime_error("Execve Error: " + string(strerror(errno)));
	}
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

int executeCGI(exec_data *info)
{
	pid_t pid;
	int	  status;

	if (pipe(info->pipefd) == -1) {
		throw std::runtime_error("Pipe Error: " + string(strerror(errno)));
	}
	pid = fork();
	if (pid < 0) {
		throw std::runtime_error("Fork Error: " + string(strerror(errno)));
	}
	else if (pid == 0) {
		childProcess(info);
	}
	else {
		close(info->pipefd[WRITE]);
		dup2(info->pipefd[READ], STDIN_FILENO);
		close(info->pipefd[READ]);
		recvCGIResponse(STDIN_FILENO, info->response);
		waitpid(pid, &status, 0);
	}
	return 0;
}