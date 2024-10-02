/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:29:39 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/02 11:31:02 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DENYMETHOD_HPP
#define DENYMETHOD_HPP

#include <ostream>
#include <string>
#include <vector>

#include "lexer/Token.hpp"

namespace statement {

class DenyMethod : public Token
{
  private:
	std::vector< std::string > _methods;

  public:
	DenyMethod();
	DenyMethod(const DenyMethod &src);
	DenyMethod(const std::vector< std::string > &methods);
	DenyMethod &operator=(const DenyMethod &src);
	~DenyMethod();

	const std::vector< std::string > &getMethods() const;
};

std::ostream &operator<<(std::ostream &o, const DenyMethod &denyMethod);

} // namespace statement

#endif // DENYMETHOD_HPP