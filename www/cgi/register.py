#!/usr/bin/env python3

import cgi
import cgitb
import sqlite3
from werkzeug.security import generate_password_hash

cgitb.enable()

form = cgi.FieldStorage()

username = form.getvalue("username")
password = form.getvalue("password")
confirm_password = form.getvalue("confirm_password")



print("Content-Type: text/html\r\n\r\n")
print("<!DOCTYPE html> <html lang=\"en\"><head> \
	<meta charset=\"UTF-8\"> \
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	<title>Create an acount</title>\
	<link rel=\"stylesheet\" href=\"/css/style.css\">\
</head>\
\
<body>\
	<div class=\"title\">\
		<h1>Register Page</h1>\
	</div>\
	<div class=\"navbar\">\
        <li><a href=\"/html/register.html\">Retry register</a></li>\
			<li><a href=\"/html/index.html\">Home</a></li></div>\
            <div class=\"title\">\
")

print(username)
print(password)
print(confirm_password)

if password != confirm_password or not username or not password:
    print("<h1>Passwords do not match!</h1></li>")
else:
    hashed_password = generate_password_hash(password)
    
    try:
        conn = sqlite3.connect('www/database/users.db')
        c = conn.cursor()
        c.execute("CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)")
        c.execute("INSERT INTO users (username, password) VALUES (?, ?)", (username, hashed_password))
        conn.commit()
        print("<br>🥳Account created successfully!🥳")
        print("<br><h1> Welcome " + username + "<br><br>🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥</h1>")
    except sqlite3.IntegrityError:
        print("<br><h1>Username already exists!</h1>")
    except Exception as e:
        print("<br><h1>Something went wrong!</h1>")
        print(e)
    finally:
        print("</div></body></html>")
        conn.close()