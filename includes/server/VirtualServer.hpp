/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 22:55:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/13 00:32:16 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>

#include "lexer/Token.hpp"
#include "parser/Location.hpp"
#include "template/vector_deep_copy.tpp"

class VirtualServer : public IConfig
{
  private:
	std::map< std::string, Param * > _config;
	std::vector< Location * >		 _locations;

  public:
	VirtualServer();
	VirtualServer(const VirtualServer &src);
	VirtualServer &operator=(const VirtualServer &rhs);
	~VirtualServer();

	bool operator==(const VirtualServer &rhs) const;

	void			addParam(const std::string &key, Param *param);
	void			addLocation(Location *location);
	const Param	   *getParam(const std::string &key) const;
	const Location *getLocation(const std::string &path) const;

	void print() const;
};

#endif // SERVER_HPP