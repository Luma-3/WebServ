/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/19 12:48:29 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <istream>

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

		Parser parser(Lexer.getTokens());
		parser.Parse();
	} catch (const std::exception &e) {
		cerr << e.what() << endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int main(const int ac, const char **av)
{
	int ret = StartWebServ(ac, av);
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
