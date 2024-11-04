/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/04 15:39:05 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>

#include "lexer/Lexer.hpp"
#include "Logger.hpp"
#include "parser/Parser.hpp"
#include "server/Handler.hpp"
#include "server/Signal.hpp"

Handler *init_server(const char **av, const char **envp)
{

	Lexer Lexer(av[1]);
	Lexer.Tokenize();

	parser::Parser parser(&Lexer);
	parser.Parse();

	const std::vector< VirtualServer * > servers = parser.getServers();
	if (!parser.getParseStack().empty()) {
		Param *token = dynamic_cast< Param * >(parser.getParseStack().top());
		new Logger(token->getPair().second,
				   Logger::StringToLogLevel(token->getPair().first));
	}

	Handler *handler = new Handler(servers, envp);

	return (handler);
}

int main(const int ac, const char **av, const char **env)
{
	Handler *handler = NULL;

	if (ac != 2) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return FAILURE;
	}
	if (initSignal() == FAILURE) {
		return (EPERM);
	}

	try {
		handler = init_server(av, env);
		handler->runEventLoop();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return FAILURE;
	}
	// } catch (const std::exception &e) {
	// 	std::cerr << e.what() << " | errno: " << strerror(errno) << '\n';
	// 	delete handler;
	// 	return (1);
	// }

	delete handler;

	// if (Logger::Instance) {
	// 	delete Logger::Instance;
	// }

	return 0;
}
