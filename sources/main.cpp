/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/09 15:10:17 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <iostream>
#include <istream>

#include "WebServ.hpp"

using std::cerr;
using std::endl;

int main(int ac, char **av)
{
	if (ac != 2) {
		cerr << "Wrong Numbre of Argument" << endl;
		return (EINVAL);
	}

	WebServ serv((std::string(av[1])));

	serv.Parse();
}
