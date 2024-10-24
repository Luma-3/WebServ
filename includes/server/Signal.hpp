/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Monsieur_Canard <Monsieur_Canard@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:24:32 by jdufour           #+#    #+#             */
/*   Updated: 2024/10/24 10:57:47 by Monsieur_Ca      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <csignal>
#include <iostream>

#define SUCCESS 0
#define FAILURE 1

extern volatile int g_sig;

void handler(int signum);
int	 initSignal();

#endif