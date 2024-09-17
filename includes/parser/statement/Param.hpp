/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:27:58 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/17 13:02:04 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMSTATEMENT_HPP
#define PARAMSTATEMENT_HPP

#include <string>

#include "lexer/Token.hpp"

namespace statement {

enum Param_Type {
	None,
	root,
	index,
	server_name,
	listen,
	autoindex,
	return_,
};

class Param : public Token
{
  private:
	Param_Type	_param_type;
	std::string _error_code;
	std::string _value;

  public:
	Param();
	Param(const Param &src);
	Param &operator=(const Param &src);
	~Param();
};

} // namespace statement

#endif // PARAMSTATEMENT_HPP