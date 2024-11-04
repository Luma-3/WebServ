# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.py                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/20 13:05:14 by Monsieur_Ca       #+#    #+#              #
#    Updated: 2024/10/31 16:19:07 by jbrousse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/env python3
import os
import time
import sys

class Style:
	content: str;
	def __init__(self, content: str = None):
		self.content = content;

	def add_content(self, content: str):
		if self.content:
			self.content += content
		else:
			self.content = content

	def to_http(self):
		if self.content:
			return "<style>" + self.content + "</style>"
		return ""

class Cookie:
	name: str;
	value: str;
	expires: str;
	path: str;
	domain: str;
	secure: bool;
	httponly: bool;
	def __init__(self, name: str, value: str, expires: str = None, path: str = None, domain: str = None, secure: bool = False, httponly: bool = False):
		self.name = name
		self.value = value
		self.expires = expires
		self.path = path
		self.domain = domain
		self.secure = secure
		self.httponly = httponly

	def to_http(self):
		cookie = f"{self.name}={self.value};"
		if self.expires:
			cookie += f"Expires={self.expires};"
		if self.path:
			cookie += f"Path={self.path};"
		if self.domain:
			cookie += f"Domain={self.domain};"
		if self.secure:
			cookie += "Secure;"
		if self.httponly:
			cookie += "HttpOnly;"
		return cookie

class Header:
	cookies: list[Cookie];
	content_type: str;
	content_length: int;
	status_code: int;
	status_message: str;
	headers: list[str];
	def __init__(self):
		self.cookies = []
		self.content_type = None
		self.content_length = None
		self.status_code = None
		self.status_message = None
		self.headers = []
	
	def set_cookies(self, name: str, value: str, expires: str = None, path: str = None, domain: str = None, secure: bool = False, httponly: bool = False):
		cookie = Cookie(name, value, expires, path, domain, secure, httponly)
		self.cookies.append(cookie)
	def add_cookie(self, cookie: Cookie):
		self.cookies.append(cookie)
	def set_content_type(self, content_type: str):
		self.content_type = content_type
	def set_content_length(self, content_length: int):
		self.content_length = content_length
	def set_status_code(self, status_code: int):
		self.status_code = status_code
	def set_status_message(self, status_message: str):
		self.status_message = status_message
	def add_header(self, header: str):
		self.headers.append(header)
	def to_http(self):
		content = ""
		if self.status_code:
			content += f"HTTP/1.1 {self.status_code} {self.status_message}\r\n"
		else :
			content += "HTTP/1.1 200 OK\r\n"
		content += "Server: WebServ/0.5 (Unix)\r\n"
		content += "Date: {}\r\n".format(time.strftime("%a, %d %b %Y %H:%M:%S GMT"))
		if self.cookies:
			for cookie in self.cookies:
				content += f"Set-Cookie: {cookie.to_http()}\r\n"
		if self.content_type:
			content += f"Content-Type: {self.content_type}\r\n"
		if self.headers:
			for header in self.headers:
				content += header + "\r\n"
		if self.content_length:
			content += f"Content-Length: {self.content_length}\r\n"
		
		return content + "\r\n"

class Head:
	content: str;
	style: Style;
	def __init__(self, content: str = "", style: Style = None):
		self.content = content;
		if style:
			self.style = style
		else:
			self.style = Style()

	def add_content(self, content: str):
		self.content += content
	
	def get_style(self):
		return self.style
	def set_style(self, style: Style):
		self.style = style
	def add_style(self, content: str):
		self.style.add_content(content)

	def to_http(self):
		content = "<head>"
		content += self.style.to_http()
		content += self.content
		content += "</head>"
		return content

class Body:
	content: str;
	def __init__(self, content: str = ""):
		self.content = content

	def add_content(self, content: str):
		self.content += content

	def to_http(self):
		return "<body>" + self.content + "</body>"

class HttpResponse:
	header: Header;
	head: Head;
	body: Body;
	def __init__(self, header: Header = None, head: Head = None, body: Body = None):
		if header:
			self.header = header
		else:
			self.header = Header()
		if head:
			self.head = head
		else:
			self.head = Head()
		if body:
			self.body = body
		else:
			self.body = Body()

	def get_header(self):
		return self.header
	def set_header(self, header: Header):
		self.header = header
	def add_header(self, header: str):
		self.header.add_header(header)
		return self
	
	def set_status_code(self, status_code: int):
		self.header.set_status_code(status_code)
		return self
	
	def set_status_message(self, status_message: str):
		self.header.set_status_message(status_message)
		return self
	
	def set_content_type(self, content_type: str):
		self.header.set_content_type(content_type)
		return self
	
	def set_content_length(self, content_length: int):
		self.header.set_content_length(content_length)
		return self
	def set_cookies(self, name: str, value: str, expires: str = None, path: str = None, domain: str = None, secure: bool = False, httponly: bool = False):
		self.header.set_cookies(name, value, expires, path, domain, secure, httponly)
		return self
	def add_cookie(self, cookie: Cookie):
		self.header.add_cookie(cookie)
		return self
	def get_head(self):
		return self.head
	def set_head(self, head: Head):
		self.head = head
		return self
	def add_head(self, head: str):
		self.head.add_content(head)
		return self
	def add_style(self, content: str):
		self.head.add_style(content)
		return self
	def get_body(self):
		return self.body
	def set_body(self, body: Body):
		self.body = body
		return self
	def add_body(self, body: str):
		self.body.add_content(body)
		return self
	def to_http(self):
		content = self.header.to_http()
		content += "<html>"
		content += self.head.to_http()
		content += self.body.to_http()
		content += "</html>"
		return content
	def print(self):
		print(self.to_http())


remote_addr = os.environ.get("REMOTE_ADDR", None)
server_name = os.environ.get("SERVER_NAME", None)
server_port = os.environ.get("SERVER_PORT", None)
server_protocol = os.environ.get("SERVER_PROTOCOL", None)
query_string = os.environ.get("QUERY_STRING", None)
cookie = os.environ.get("HTTP_COOKIE", None)

# time.sleep(11) #simulate a long task TimeOut

((HttpResponse())
.set_cookies("jesuisuncookie", "42")
.set_cookies("jesuisuncookies", "42d")
.set_status_code(404)
.set_status_message("OKY")
.add_body("<p>REMOTE_ADDR: {}</p>".format(remote_addr))
.add_body("<p>SERVER_NAME: {}</p>".format(server_name))
.add_body("<p>SERVER_PORT: {}</p>".format(server_port))
.add_body("<p>SERVER_PROTOCOL: {}</p>".format(server_protocol))
.add_body("<p>QUERY_STRING: {}</p>".format(query_string))
.add_body("<p>HTTP_COOKIE: {}</p>".format(cookie))
.set_content_type("text/html")
.add_head("<title>Test CGI</title>")
.add_style("body {background-color: powderblue;}")
.add_body("<h1>Test CGI</h1>")
.add_body("<p>TIME: {}</p>".format(time.strftime("%Y-%m-%d %H:%M:%S")))\
.print())

