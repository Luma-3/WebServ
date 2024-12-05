/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/12/05 10:07:02 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <exception>
#include <iostream>

#include "lexer/Lexer.hpp"
#include "Logger.hpp"
#include "parser/Parser.hpp"
#include "server/Handler.hpp"
#include "server/Signal.hpp"

using parser::Parser;

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

Handler *init_server(const char *conf_file, const char **envp)
{

	Lexer Lexer(conf_file);
	Lexer.Tokenize();

	Parser parser(&Lexer);
	try {
		parser.Parse();
	} catch (const std::exception &e) {
		const vector< VirtualServer * > &servers = parser.getServers();
		for (size_t i = 0; i < servers.size(); i++) {
			delete servers[i];
		}
		throw;
	}

	const vector< VirtualServer * > &servers = parser.getServers();

	if (!parser.getParseStack().empty()) {
		Param *token = D_Cast< Param * >(parser.getParseStack().top());
		new Logger(token->getPair().second,
				   Logger::StringToLogLevel(token->getPair().first));
	}

	Handler *handler = new Handler(servers, envp);

	return (handler);
}

void delete_instance(Handler *handler, Logger *logger)
{
	delete handler;
	delete logger;
}

int main(const int ac, const char **av, const char **env)
{
	Handler *handler = NULL;

	cout << "Starting server..." << endl;
	if (ac != 2) {
		cerr << "Usage: ./webserv <config_file>" << endl;
		return FAILURE;
	}
	if (initSignal() == FAILURE) {
		return (EPERM);
	}

	try {
		handler = init_server(av[1], env);
		cout << "All server started !" << endl;
		handler->runEventLoop();
	} catch (const std::exception &e) {
		cerr << e.what() << endl;
		delete_instance(handler, Logger::Instance);
		return FAILURE;
	}

	delete_instance(handler, Logger::Instance);
	return SUCCESS;
}
