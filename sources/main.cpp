/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/01 14:53:52 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "parser/statement/Server.hpp"
#include "server/Handler.hpp"
#include "server/Server.hpp"
#include "server/Signal.hpp"

// int StartWebServ(const int ac, const char **av)
// {
// 	if (ac != 2) {
// 		cerr << "Wrong Numbre of Argument" << endl;
// 		return (EINVAL);
// 	}

// 	try {
// 		Lexer Lexer(av[1]);
// 		Lexer.Tokenize();

// 		std::vector< Token * > tokens = Lexer.getTokens();

// 		for (size_t i = 0; i < tokens.size(); i++) {
// 			std::cout << *tokens[i] << std::endl;
// 		}

// 		parser::Parser parser(Lexer.getTokens());
// 		parser.Parse();

// 		parser.getParseStack();
// 		statement::Server *server =
// 			dynamic_cast< statement::Server * >(parser.getParseStack().top());
// 		std::cout << *server << std::endl;
// 	} catch (const std::exception &e) {
// 		cerr << e.what() << endl;
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }

Handler *init_server(const int ac, const char **av)
{
	if (ac != 2) {
		throw std::runtime_error("Wrong Numbre of Argument");
	}

	Lexer Lexer(av[1]);
	Lexer.Tokenize();

	std::vector< Token * > tokens = Lexer.getTokens();

	parser::Parser parser(&Lexer);
	parser.Parse();

	std::vector< statement::Server * > servers;
	std::stack< Token * >			   stack = parser.getParseStack();

	while (!stack.empty()) {
		statement::Server *server =
			dynamic_cast< statement::Server * >(stack.top());
		servers.push_back(server);
		stack.pop();
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
		handler->launchServers();
		handler->handleEvents();
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << '\n';
		return (1);
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		delete handler;
		return (1);
	}

	delete handler;

	return 0;
}
