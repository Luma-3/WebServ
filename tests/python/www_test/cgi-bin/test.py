import cgi
import os
# import cgitb
# cgitb.enable()

print("Content-Type: text/html\r\n\r\n")

form = cgi.FieldStorage()
name = form.getvalue('name')
age = form.getvalue('age')

cookies = os.environ.get('HTTP_COOKIE', '')

print(f"name={name}&age={age}&cookie={cookies}")