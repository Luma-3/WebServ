#!/usr/bin/env python3

import cgi
import cgitb

cgitb.enable()
form = cgi.FieldStorage()

if 'fiawdawdawdle' in form:

	file_field = form["file"]
	body = file_field.file.read()
	filename = file_field.filename
	file_path = filename
	try:
		with open(file_path, 'wb') as file:
			file.write(body)
			message = 'File uploaded successfully'
	except IOError as e:
		message = 'Error uploading file: ' + e
else:
	message = 'No file uploaded'