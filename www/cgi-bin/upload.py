#!/usr/bin/env python3

import cgi
import os

upload_dir = './www/upload_files/'

if not os.path.exists(upload_dir):
	os.makedirs(upload_dir)

form = cgi.FieldStorage()

if 'file' in form:

	file_field = form["file"]
	body = file_field.file.read()
	filename = file_field.filename
	file_path = upload_dir + filename
	try:
		with open(file_path, 'wb') as file:
			file.write(body)
			message = 'File uploaded successfully'
	except IOError as e:
		message = 'Error uploading file: ' + e
else:
	message = 'No file uploaded'

print ('Content-type: text/html\r\n\r\n')
print(f'<h1>{message}</h1>')