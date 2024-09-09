/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:27:20 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/09 10:28:53 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLient
#define CLient

#include <string>
#include <map>
#include "Server.hpp"

class Client : public Server
{
  private:
	const int							_socket;
	int									_bytes_read;
	std::map<std::string, std::string>	_request;	
	std::string 						_response;

  public:
	Client();

	int		GETRessource();
	int		POSTRessource();
	char	*createResponse();	

	~Client();
};

#endif // CLient