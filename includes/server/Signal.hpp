/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:24:32 by jdufour           #+#    #+#             */
/*   Updated: 2024/11/27 11:07:38 by jbrousse         ###   ########.fr       */
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
