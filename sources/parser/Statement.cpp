/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Statement.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:18:34 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/14 12:18:53 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Statement.hpp"

Statement::Statement() {}

Statement::Statement(const Statement &src) {}

Statement &Statement::operator=(const Statement &src)
{
	if (this != &src) {}
	return *this;
}

Statement::~Statement() {}