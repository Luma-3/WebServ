/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Statement.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:03:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/13 15:07:25 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATEMENT_HPP
#define STATEMENT_HPP

class Statement
{
  private:

  public:
	Statement();
	Statement(const Statement &src);
	Statement &operator=(const Statement &src);
	~Statement();
};

Statement::Statement() {}

Statement::Statement(const Statement &src) {}

Statement &Statement::operator=(const Statement &src)
{
	if (this != &src) {}
	return *this;
}

Statement::~Statement() {}

#endif // STATEMENT_HPP