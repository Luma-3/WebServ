/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Statement.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:03:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/14 12:18:52 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATEMENT_HPP
#define STATEMENT_HPP

enum Statement_Type {
	ServerParam,
	LocationParam,
	ErrorPage,
	DenyMethod,
	LocationBlock
};

class Statement
{
  protected:
	Statement_Type _type;

  public:
	Statement();
	Statement(const Statement &src);
	Statement &operator=(const Statement &src);
	~Statement();
};



#endif // STATEMENT_HPP