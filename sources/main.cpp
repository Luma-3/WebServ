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
	// The servers instanciation will happen in there. They will all be pushed
	// back into a vector. The Config struct and its function will be called in
	// there. It probably will need to be initialized in main and therefore be
	// passed as an argument to the loadServ() method ?
	serverVect.loadServTest();

	// All of the infra happens in there
	if (serverVect.launchServers()) {
		return (1);
	}

	return (0);
}