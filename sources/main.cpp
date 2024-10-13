/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/13 11:07:16 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "server/Handler.hpp"
#include "server/Signal.hpp"

Handler *init_server(const int ac, const char **av)
{
	if (ac != 2) {
		throw std::runtime_error("Wrong Numbre of Argument");
	}

	Lexer Lexer(av[1]);
	Lexer.Tokenize();

	std::cout << "Token size MAIN: " << Lexer.getTokens().size() << std::endl;

	parser::Parser parser(&Lexer);
	parser.Parse();

	const std::vector< VirtualServer * > servers = parser.getServers();

	std::vector< VirtualServer * >::const_iterator it = servers.begin();
	while (it != servers.end()) {
		std::cout << "Server: " << std::endl;
		(*it)->print();
		++it;
	}

	Handler *handler = new Handler(servers);

	return (handler);
}

int main(const int ac, const char **av, const char **env)
{
	(void)env;
	Handler *handler = NULL;

	initSignal();

	try {
		handler = init_server(ac, av);
		handler->runEventLoop();
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << " | errno: " << strerror(errno) << '\n';
		return (1);
	} catch (const std::exception &e) {
		std::cerr << e.what() << " | errno: " << strerror(errno) << '\n';
		delete handler;
		return (1);
	}

	delete handler;

	return 0;
}
