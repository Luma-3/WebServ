/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:27:58 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 14:28:20 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMSTATEMENT_HPP
#define PARAMSTATEMENT_HPP

#include <string>

#include "lexer/Token.hpp"

namespace statement {

class Param : public Token
{
  private:
	const std::string _value;

  public:
	Param();
	Param(const Param &src);
	Param(const std::string &value, enum Terminal_Type key);
	Param &operator=(const Param &src);

	// static Param_Type IdentifyParam(const std::string &key);
	bool ConvertBool(const std::string &key);

	const std::string &getValue() const;

	~Param();
};

} // namespace statement

#endif // PARAMSTATEMENT_HPP