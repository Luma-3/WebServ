/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 17:34:55 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Client.hpp"

int main()
{
	std::string request = "DELETE /index HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 20\n";

	Client client;
	try {
		client.parseRequest((char *)request.c_str());
	} catch (InvalidRequestException &e) {
		std::cerr << e.what() << std::endl;
	}

	for (std::map<std::string, std::string>::iterator it = client._headers.begin(); it != client._headers.end(); it++)
	{
		std::cout << it->first << "/ => " << it->second << std::endl;
	}
}