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

class Client
{
  private:
	const int	socket;
	int			bytes_read;
	std::string response;

  public:
	Client();
	~Client();
};

#endif // CLient