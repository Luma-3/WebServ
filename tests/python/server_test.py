import unittest
import subprocess
import requests
import time

class TestServerStaticStatusWebsiteConf(unittest.TestCase):
	@classmethod
	def setUpClass(cls):
		cls.server_process = subprocess.Popen(['./build/WebServ', 'tests/python/conf/classic.conf'],
										cwd='../../',
										stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		time.sleep(1)

		cls.url = 'http://localhost:8080'

	@classmethod
	def tearDownClass(cls):
		cls.server_process.send_signal(subprocess.signal.SIGTERM)
		cls.server_process.wait()

	def test_index(self):
		response = requests.get(self.url)
		self.assertEqual(response.status_code, 200)

	# def test_not_found(self):
	# 	response = requests.get(self.url + "/ldjalkdjwalkwdj")
	# 	self.assertEqual(response.status_code, 404)

	# def test_bad_request(self):
	# 	response = requests.put(self.url)
	# 	self.assertEqual(response.status_code, 405)

	# def test_no_perm(self):
	# 	response = requests.get(self.url + "/file_no_perm/")
	# 	self.assertEqual(response.status_code, 403)


if __name__ == '__main__':
	unittest.main()