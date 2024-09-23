/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/23 14:15:39 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "parser/statement/Server.hpp"

using std::cerr;
using std::endl;

int StartWebServ(const int ac, const char **av)
{
	if (ac != 2) {
		cerr << "Wrong Numbre of Argument" << endl;
		return (EINVAL);
	}

	try {
		Lexer Lexer(av[1]);
		Lexer.Tokenize();

		std::vector< Token * > tokens = Lexer.getTokens();

		for (size_t i = 0; i < tokens.size(); i++) {
			std::cout << *tokens[i] << std::endl;
		}

		parser::Parser parser(Lexer.getTokens());
		parser.Parse();

		parser.getParseStack();
		statement::Server *server =
			dynamic_cast< statement::Server * >(parser.getParseStack().top());
		std::cout << *server << std::endl;
	} catch (const std::exception &e) {
		cerr << e.what() << endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int main(const int ac, const char **av)
{
	const int ret = StartWebServ(ac, av);
	if (ret != 0) {
		return (ret);
	}

	// std::stack< Token * > stack = parser.getParseStack();

	// std::cout << "Stack size: " << stack.size() << std::endl;
	// while (!stack.empty()) {
	// 	statement::Server *server =
	// 		dynamic_cast< statement::Server * >(stack.top());
	// 	std::cout << *server << std::endl;
	// 	stack.pop();
	// }
}
