/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/10 20:03:55 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <iostream>
#include <istream>

#include "Parser.hpp"
#include "WebServ.hpp"

using std::cerr;
using std::endl;

int main(int ac, char **av)
{
	if (ac != 2) {
		cerr << "Wrong Numbre of Argument" << endl;
		return (EINVAL);
	}

	Parser parser(av[1]);
}
