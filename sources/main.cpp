/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/16 15:40:50 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <istream>

#include "parser/Lexer.hpp"
#include "parser/Parser.hpp"

using std::cerr;
using std::endl;

int main(const int ac, const char **av)
{
	if (ac != 2) {
		cerr << "Wrong Numbre of Argument" << endl;
		return (EINVAL);
	}

	try {
		Lexer lexer(av[1]);

		lexer.Tokenize();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
}
