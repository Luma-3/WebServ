/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:33:51 by jdufour           #+#    #+#             */
/*   Updated: 2024/09/12 23:41:38 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Handler.hpp"

Handler::Handler() {}

// void	Handler::loadServ()
// {
// 	Config	serverConf = Config.getConf(); //to be replaced by the real function
// 	_nbServ =  serverConf.getNbServers(); //idem
	
// 	for (int i = 0; i < _nbServ; i++) 
// 	{
// 		// these 3 will be rewritten when the config class will be set
// 		std::string	name = serverConf.getServerName(i);
// 		std::string	hostname = serverConf.getHostname(i);
// 		std::string	port = serverConf.getPort(i);
// 		Server	server = Server( name, hostname, port);
// 		_servers.push_back(server);
// 	}
// }

void	Handler::loadServTest()
{
	_nbServ = 4;
	std::string	names[4] = {"server1", "server2", "server3", "server4"};
	std::string ports[4] = {"8080", "8081", "8082", "9092"};
	
	for (int i = 0; i < _nbServ; i++) 
	{
		std::string	name = names[i];
		std::string	port = ports[i];
		_servers.push_back(new Server( name, "localhost", port));
	}
}

Server	*Handler::operator[]( const int index)
{
	if (index < 0 || index >= _nbServ || !_nbServ)
		throw( std::out_of_range("Error : invalid access index on Servers"));
	return (_servers[index]);
}

std::vector<Server*>	Handler::getAllServ( void) const
{
	return (_servers);
}


int	Handler::getNbServ( void) const
{
	return (_nbServ);
}

Handler::~Handler() 
{
	for (int i = 0; i < _nbServ; ++i)
		delete _servers[i];
}