/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:27:56 by jdufour           #+#    #+#             */
/*   Updated: 2024/09/24 15:04:02 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Signal.hpp"

void handler(int signum)
{
	if (signum == SIGINT) {
		g_sig = SIGINT;
	}
}

void initSignal()
{
	signal(SIGINT, handler);
}