/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:19:07 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/23 15:05:45 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Handler.hpp"
#include "Server.hpp"
#include "Signal.hpp"

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