/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/19 21:44:21 by anthony          ###   ########.fr       */
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
	// std::string header = "GET / HTTP/1.1\n";
	// std::string host = "Host: localhost:8080\n";
	// std::string contentType = "Content-Type: text/html\n";
	// std::string contentLength = "Content-Length: 10\n";

	// std::vector< std::string > headers;
	// headers.push_back(header);
	// headers.push_back(host);
	// headers.push_back(contentType);
	// headers.push_back(contentLength);

	int				   server_sock = socket(AF_INET, SOCK_STREAM, 0);
	int				   server_port = 8080;
	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

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
		std::cout << "buffer : " << buff << std::endl;
		client.getParser().parseRequest(buff);
		client.buildResponse();
		memset(buff, 0, BUFFER_SIZE);
		(void)valread;
		std::string response = client.buildResponse();
		send(client_sock, response.c_str(), response.length(), 0);
		close(client_sock);
	}
}