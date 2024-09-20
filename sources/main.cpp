/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:19:07 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 22:49:38 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Handler.hpp"
#include "../includes/Server.hpp"
#include "../includes/Signal.hpp"

int main(void)
{
	// if (argc != 2)
	// 	return (1);

	Handler serverVect = Handler();
	serverVect.loadServTest();
	if (serverVect.launchServers()) {
		return (1);
	}

	serverVect.handleEvents();

	return (0);
}