/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/11 19:48:18 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <iostream>
#include <istream>

#include "parser/Parser.hpp"

using std::cerr;
using std::endl;

int main(const int ac, const char **av)
{
	if (ac != 2) {
		cerr << "Wrong Numbre of Argument" << endl;
		return (EINVAL);
	}

	Parser parser(av[1]);

	parser.Tokenize();
}
