/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:29:39 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/21 11:42:01 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DENYMETHOD_HPP
#define DENYMETHOD_HPP

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

} // namespace statement

#endif // DENYMETHOD_HPP