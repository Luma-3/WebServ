#!/usr/bin/env python3

import cgi
import cgitb
import sqlite3
from werkzeug.security import generate_password_hash

cgitb.enable()

# print("Content-Type: text/html")
# print()

form = cgi.FieldStorage()

username = form.getvalue("username")

print("Content-Type: text/html\r\n\r\n")

password = form.getvalue("password")
confirm_password = form.getvalue("confirm_password")

if password != confirm_password:
    print("<h1>Passwords do not match!</h1>")
else:
    hashed_password = generate_password_hash(password)
    
    try:
        conn = sqlite3.connect('users.db')
        c = conn.cursor()
        c.execute("CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)")
        c.execute("INSERT INTO users (username, password) VALUES (?, ?)", (username, hashed_password))
        conn.commit()
        print("<h1>Account created successfully!</h1>")
    except sqlite3.IntegrityError:
        print("<h1>Username already exists!</h1>")
    except Exception as e:
        print("<h1>Something went wrong!</h1>")
        print(e)
    finally:
        conn.close()