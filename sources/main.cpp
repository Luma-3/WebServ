/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 16:16:41 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <istream>

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

using std::cerr;
using std::endl;

int main(const int ac, const char **av)
{
	if (ac != 2) {
		cerr << "Wrong Numbre of Argument" << endl;
		return (EINVAL);
	}

	// try {
	// 	Lexer lexer(av[1]);

	// 	lexer.Tokenize();
	// } catch (std::exception &e) {
	// 	std::cerr << e.what() << std::endl;
	// 	return (EXIT_FAILURE);
	// }

	Lexer Lexer(av[1]);

	Lexer.Tokenize();

	Parser parser(Lexer.getTokens());

	parser.Parse();

	std::stack< Token * > stack = parser.getParseStack();

	while (!stack.empty()) {
		std::cout << stack.top() << std::endl;
		stack.pop();
	}
}
