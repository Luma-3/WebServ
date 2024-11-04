/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 22:55:01 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/04 15:13:05 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>

#include "lexer/Token.hpp"
#include "parser/Location.hpp"
#include "template/VectorDeepCopy.tpp"

class VirtualServer : public IConfig
{
  private:
	std::map< std::string, Param * >	_config;
	std::map< std::string, Location * > _locations;

  public:
	VirtualServer();
	VirtualServer(const VirtualServer &src);
	VirtualServer &operator=(const VirtualServer &rhs);
	~VirtualServer();
	bool operator==(const VirtualServer &rhs) const;

	void addParam(const std::string &key, Param *param);
	// const Location *getLocation(const std::string &path) const;

	void		 addLocation(const std::string &route, Location *location);
	const Param *getParam(const std::string &key) const;
	std::string	 getParamValue(const std::string &key) const;
	std::pair< std::string, std::string >
									 getParamPair(const std::string &key) const;
	std::vector< std::string > getParamList(const std::string &key) const;
	const Location					*getLocation(const std::string &path) const;
	std::string						 getRoot(const std::string &path) const;

	void print() const;
};

#endif // SERVER_HPP