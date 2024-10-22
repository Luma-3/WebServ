#!/usr/bin/env python3

import cgi
import cgitb
import sqlite3
from werkzeug.security import generate_password_hash

cgitb.enable()

print("Content-Type: text/html")
print()

form = cgi.FieldStorage()

username = form.getvalue("username")
password = form.getvalue("password")
confirm_password = form.getvalue("confirm_password")

if password != confirm_password:
    print("<h1>Passwords do not match!</h1>")
else:
    hashed_password = generate_password_hash(password, method='sha256')

    try:
        conn = sqlite3.connect('users.db')
        c = conn.cursor()
        c.execute("INSERT INTO users (username, password) VALUES (?, ?)", (username, hashed_password))
        conn.commit()
        conn.close()
        print("<h1>Account created successfully!</h1>")
    except sqlite3.IntegrityError:
        print("<h1>Username already exists!</h1>")