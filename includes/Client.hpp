/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:27:20 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/18 22:49:49 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLient
#define CLient

#include <map>
#include <string>

#include "Server.hpp"

class Client
{
  private:
	const int	_socket;
	int			_bytes_read;
	char	   *_request;
	std::string _response;

  public:
	Client();
	Client(const char *request);

	std::string getResponse() const;
	void		HandleRequest();

	~Client();
};

#endif // CLient