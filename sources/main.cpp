/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/16 16:32:37 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Client.hpp"
#include "Server.hpp"

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

	if (listen(server_sock, 10) == -1) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return 1;
	}

	std::cout << "Server listening on port " << server_port << std::endl;
	Server server;

	while (true) {
		Client client(server);
		int	   client_sock = accept(server_sock, NULL, NULL);
		char   buff[1024] = {0};
		int	   valread = recv(client_sock, buff, 1024, 0);

		try {
			client.parseRequest(buff);
		} catch (InvalidRequestException &e) {
			std::cerr << e.what() << std::endl;
		}
		memset(buff, 0, 1024);
		(void)valread;
		std::string response = client.buildResponse();
		send(client_sock, response.c_str(), response.length(), 0);
		close(client_sock);
	}
}