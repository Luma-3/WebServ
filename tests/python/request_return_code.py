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

class TestServerStaticReturnCodeWebsiteConf(TestFunct):
	@classmethod
	def setUpClass(cls):
		with open('.server_pid', 'r') as f:
			server_pid = f.read()
		cls.url = 'http://localhost:8080'

##################################################
# Test GET
##################################################
	def test_index(self):
		print("Testing index page, should return 200")
		response = requests.get(self.url + "/")
		self.verifReturn(response, 200)

	def test_not_found(self):
		print("Testing not found page, should return 404")
		response = requests.get(self.url + "/index.htmoul")
		self.verifReturn(response, 404)

##################################################
# Test DELETE
##################################################
	def test_delete(self):
		print("Testing delete, should return 200")
		response = requests.delete(self.url + "/delete/")
		self.verifReturn(response, 200)


##################################################
# Test Permission
##################################################
	def test_no_permission(self):
		print("Testing no permission, should return 403")
		response = requests.get(self.url + "/no_permission/")
		self.verifReturn(response, 403)

##################################################
# Test Autoindex
##################################################
	def test_no_autoindex(self):
		print("Testing no autoindex, should return 403")
		response = requests.get(self.url + "/autoindex-off/")
		self.verifReturn(response, 403)

	def test_autoindex(self):
		print("Testing autoindex, should return 200")
		response = requests.get(self.url + "/autoindex-on/")
		self.verifReturn(response, 200)

##################################################
# Test Redirection
##################################################
	def test_redirect_302(self):
		print("Testing redirect 302, should return 302")
		response = requests.get(self.url + "/redirect-302/", allow_redirects=False)
		self.verifReturn(response, 302)
	
	def test_redirect_301(self):
		print("Testing redirect 301, should return 301")
		response = requests.get(self.url + "/redirect-301/", allow_redirects=False)
		self.verifReturn(response, 301)


	def test_redirect(self):
		print("Testing redirect final code, should return 200")
		response = requests.get(self.url + "/redirect-301/")
		self.verifReturn(response, 200)

##################################################
# Test Internal Server Error
##################################################
	def test_internal_server_error_broken_script(self):
		print("Testing internal server error, should return 500")
		response = requests.get(self.url + "/cgi-broken/")
		self.verifReturn(response, 500)

##################################################
# Test Deny-Methods
##################################################
	def test_method_not_allowed(self):
		print("Testing put method not allowed, should return 405")
		response = requests.put(self.url)
		self.verifReturn(response, 405)

	def test_deny_method_delete(self):
		print("Testing delete deny method, should return 405")
		response = requests.delete(self.url + "/deny-delete/")
		self.verifReturn(response, 405)

	def test_deny_method_post(self):
		print("Testing post deny method, should return 405")
		response = requests.post(self.url + "/deny-post/")
		self.verifReturn(response, 405)

	def test_deny_method_get(self):
		print("Testing get deny method, should return 405")
		response = requests.get(self.url + "/deny-get/")
		self.verifReturn(response, 405)

##################################################
# Test CGI
##################################################
	def test_wrong_cgi_good_script(self):
		print("Testing wrong cgi, should return 500")
		response = requests.post(self.url + "/cgi-wrong/")
		self.verifReturn(response, 500) 
	# TODO : to delete test is not correct



	def test_cgi_script(self):
		print("Testing cgi script, should return 200")
		response = requests.get(self.url + "/cgi-good/")
		self.verifReturn(response, 200)


	def test_cgi_script_post(self):
		print("Testing cgi script post, should return 200")
		response = requests.post(self.url + "/cgi-good/")
		self.verifReturn(response, 200)

if __name__ == '__main__':
	unittest.main()