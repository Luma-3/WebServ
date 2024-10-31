#!/usr/bin/env python3

import cgi
import cgitb
import os

cgitb.enable()

UPLOAD_DIR = 'www/uploads/'

if not os.path.exists(UPLOAD_DIR):
    os.makedirs(UPLOAD_DIR)

form = cgi.FieldStorage()

if "file" not in form:
    print("Content-Type: text/html\r\n\r\n")  
    print() 
    print("<h1>Error : no file uploaded.</h1>")
else:
    fileitem = form["file"]

    if fileitem.filename:
        filepath = os.path.join(UPLOAD_DIR, os.path.basename(fileitem.filename))
        
        try:
            with open(filepath, 'wb') as f:
                f.write(fileitem.file.read())
            
            print("Content-Type: text/html\r\n\r\n")
            print(f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Success</title>
    <link rel="stylesheet" href="/css/style.css">
    <style>
        .result {{
            text-align: center;
            font-size: 2rem; 
            font-weight: bold;
            background-color: #add8e6;
            padding: 10px;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }}
    </style>
</head>
<body>
    <div class="title">
        <h1>SUCCESS</h1>
    </div>
    <div class="navbar">
        <ul>
            <li>
                <div class="calculator">
                    <h2 style="text-align: center;">File {fileitem.filename} successfully uploaded !</h2>
                    <a href="/html/index.html" style="text-align: center; display: block;">Home</a>
                </div>
            </li>
        </ul>
    </div>
</body>
</html>""")
        except Exception as e:
            print("Content-Type: text/html")
            print()
            print(f"<h1>Error encountered while uploading file : {str(e)}</h1>")
    else:
        print("Content-Type: text/html")
        print()
        print("<h1>Error : empty file.</h1>")
