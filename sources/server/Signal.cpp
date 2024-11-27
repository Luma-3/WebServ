/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:27:56 by jdufour           #+#    #+#             */
/*   Updated: 2024/11/27 11:04:13 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Signal.hpp"

void handler(int signum)
{
	if (signum == SIGINT) {
		g_sig = SIGINT;
	}
}

int initSignal()
{
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR ||
		signal(SIGINT, handler) == SIG_ERR) {
		std::cerr << "Error: signal" << std::endl;
		return FAILURE;
	}
	return SUCCESS;
}
