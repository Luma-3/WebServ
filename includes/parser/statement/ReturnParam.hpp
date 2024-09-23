/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnParam.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:38:58 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/21 11:42:19 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RETURNPARAM_HPP
#define RETURNPARAM_HPP

#include <string>
#include <vector>

#include "lexer/Token.hpp"

namespace statement {

class ReturnParam : public Token
{
  private:
	std::string _error_code;
	std::string _value;

  public:
	ReturnParam();
	ReturnParam(const ReturnParam &src);
	ReturnParam(const std::string &error_code, const std::string &value);
	ReturnParam &operator=(const ReturnParam &src);
	~ReturnParam();
};

} // namespace statement

#endif // RETURNPARAM_HPP