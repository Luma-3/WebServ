/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/20 17:23:51 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Client.hpp"
#include "Parser.hpp"
#include "Server.hpp"

#define BUFFER_SIZE 1024
#define NB_CLIENTS	10

int main()
{
	int				   server_sock = socket(AF_INET, SOCK_STREAM, 0);
	int				   server_port = 8080;
	struct sockaddr_in server_addr;
	int				   opt = 1;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		_exit(1);
	}

	bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (server_sock == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return 1;
	}

	if (listen(server_sock, NB_CLIENTS) == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return 1;
	}

	std::cout << "Server listening on port " << server_port << std::endl;
	Server server;

	while (true) {

		client::Client client(server);
		int			   valread = 0;
		int			   client_sock = accept(server_sock, NULL, NULL);
		char		   buff[BUFFER_SIZE] = {0};
		valread = recv(client_sock, buff, BUFFER_SIZE, 0);
		if (buff[0] == '\0') {
			close(client_sock);
			continue;
		}
		std::cout << "buffer : " << buff << std::endl;
		client.getParser().parseRequest(buff);
		memset(buff, 0, BUFFER_SIZE);
		(void)valread;
		std::string response = client.buildResponse();
		send(client_sock, response.c_str(), response.length(), 0);
		close(client_sock);
	}
}