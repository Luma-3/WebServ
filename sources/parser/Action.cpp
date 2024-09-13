/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 14:05:45 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/13 14:11:36 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <parser/Action.hpp>
#include <stack>
#include <string>

Action::Action() {}

Action::Action(const Action &src) {}

Action &Action::operator=(const Action &src)
{
	if (this != &src) {}
	return *this;
}

void shift(std::string &value, std::stack< Token > &stack, )
{
	std::cout << "Shift: " << value << std::endl;
	stack.push(Token(value, none));
}

Action::~Action() {}