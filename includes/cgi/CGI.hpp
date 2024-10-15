/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:00:38 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/15 16:52:00 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <map>
#include <string>
#include <unistd.h>

#include "client/Client.hpp"
#include "client/Parser.hpp"
#include "server/VirtualServer.hpp"

#define WRITE 1
#define READ  0

class Client;

void executeCGI(char *cgi, char **argv, char **envp, std::string &response);
void initCGI(const client::Parser &parser, const VirtualServer &server,
			 const client::Client &client, std::string &response);
#endif // CGI_HPP