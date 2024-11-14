import unittest
import subprocess
import requests
import time

class TestFunct(unittest.TestCase):
	def printKO(self):
		print("\033[91m======KO======\033[0m")

	def printOK(self):
		print("\033[92m======OK=======\033[0m")
	
	def verifReturn(self, response, return_code):
		try:
			self.assertEqual(response.status_code, return_code)
			self.printOK()
		except AssertionError as e:
			self.printKO()
			print(f"AssertionError: {e}" + '\n')
			exit (1)
		print()

	def verifBody(self, response, body):
		try:
			self.assertEqual(response.status_code, 200)
			self.assertIn(body, response.text)
			self.printOK()
		except AssertionError as e:
			self.printKO()
			print(f"AssertionError: {e}" + '\n')
			exit (1)
		print()


class TestServerBodyReturn(TestFunct):
	@classmethod
	def setUpClass(cls):
		with open('.server_pid', 'r') as f:
			server_pid = f.read()
		cls.url = 'http://localhost:8080'


##################################################
# Test Body Simple GET
##################################################

	def test_body_simple_get(self):
		print("Testing body simple get, body should contain '<h1>Hola !</h1>'")
		response = requests.get(self.url + "/")
		self.verifBody(response,'<h1>Hola !</h1>')


##################################################
# Test CGI with GET and POST Method and Max Body Size
##################################################

	def test_wrong_header_cgi(self):
		print("Testing wrong header CGI, should return 500")
		response = requests.get(self.url + "/cgi-bin/test_wrong_header.py")
		self.verifReturn(response, 500)

	def test_cgi_get_query_string(self):
		print("Testing CGI GET, cgi should access to query string")
		response = requests.get(self.url + "/cgi-bin/test.py?name=coucou")
		self.verifBody(response,'name=coucou&age=None')

	def test_cgi_post_query_string(self):
		print("Testing CGI POST query string, body should contain 'name=Billy&age=None'")
		data = {'name':'Billy', 'age':'None'}
		response = requests.post(self.url + "/cgi-bin/test.py", data=data)
		self.verifBody(response,'name=Billy&age=None')

	def test_max_body_size_post(self):
		print("Testing max body size, should return 413")
		data = {'name':'Billy', 'age':'None'}
		response = requests.post(self.url + "/max_size/" , data=data)
		self.verifReturn(response, 413)

##################################################
# Test Cookies
##################################################
	def test_cookie_post(self):
		print("Testing cookie with POST")
		data = {'name':'Billy', 'age':'None'}
		cookies = {'session_id':'123456'}
		response = requests.post(self.url + "/cgi-bin/test.py", cookies=cookies, data=data)
		self.verifBody(response,'name=Billy&age=None&cookie=session_id=123456')

	def test_get_with_query_and_cookies(self):
		print("Testing GET with query string and cookies")
		response = requests.get(self.url + "/cgi-bin/test.py?name=coucou", cookies={'session_id':'123456'})
		self.verifBody(response, 'name=coucou&age=None&cookie=session_id=123456')


if __name__ == '__main__':
	unittest.main()