/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:26:50 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/27 11:07:19 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICONFIG_HPP
#define ICONFIG_HPP

#include <string>
#include <vector>

#include "parser/Param.hpp"

class IConfig
{
  public:
	virtual void		 addParam(const std::string &key, Param *param) = 0;
	virtual const Param *getParam(const std::string &key) const = 0;
	virtual void		 print() const = 0;

	virtual ~IConfig() {};
};

#endif // ICONFIG_HPP
