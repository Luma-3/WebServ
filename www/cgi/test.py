# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.py                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/20 13:05:14 by Monsieur_Ca       #+#    #+#              #
#    Updated: 2024/10/28 14:27:46 by jbrousse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/env python3

import os
import time

remote_addr = os.environ["REMOTE_ADDR"]
server_name = os.environ["SERVER_NAME"]
server_port = os.environ["SERVER_PORT"]
server_protocol = os.environ["SERVER_PROTOCOL"]
query_string = os.environ["QUERY_STRING"]

# time.sleep(11) #simulate a long task TimeOut

print("Content-Type: text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Test CGI</title>")
print("</head>")
print("<body>")
print("<h1>Test CGI</h1>")
print("<p>REMOTE_ADDR: {}</p>".format(remote_addr))
print("<p>SERVER_NAME: {}</p>".format(server_name))
print("<p>SERVER_PORT: {}</p>".format(server_port))
print("<p>SERVER_PROTOCOL: {}</p>".format(server_protocol))
print("<p>QUERY_STRING: {}</p>".format(query_string))
print("</body>")
print("</html>")
