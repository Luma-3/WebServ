/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:27:56 by jdufour           #+#    #+#             */
/*   Updated: 2024/09/18 22:49:34 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Signal.hpp"

void handler(int signum)
{
	if (signum == SIGINT) {
		g_sig = 130;
	}
}

void initSignal(void)
{
	signal(SIGINT, handler);
}