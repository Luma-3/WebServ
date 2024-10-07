/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParamDouble.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:38:58 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/07 18:29:06 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMDOUBLE_HPP
#define PARAMDOUBLE_HPP

#include <string>
#include <vector>

#include "lexer/Token.hpp"

namespace statement {

class ParamDouble : public Token
{
  private:
	std::string _value1;
	std::string _value2;

  public:
	ParamDouble();
	ParamDouble(const ParamDouble &src);
	ParamDouble(const std::string &value1, const std::string &value2,
				Token_Type type);
	ParamDouble &operator=(const ParamDouble &src);
	~ParamDouble();

	const std::string &getValue1() const;
	const std::string &getValue2() const;
};

} // namespace statement

#endif // PARAMDOUBLE_HPP