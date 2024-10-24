/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:27:56 by jdufour           #+#    #+#             */
/*   Updated: 2024/10/24 11:02:41 by Monsieur_Ca      ###   ########.fr       */
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