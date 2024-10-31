#!/usr/bin/env python3

import cgi
import cgitb

cgitb.enable()

print("Content-Type: text/html\r\n\r\n")

form = cgi.FieldStorage()

print(form)
try:
    num1 = float(form.getvalue("num1", 0))
    num2 = float(form.getvalue("num2", 0))
    operateur = form.getvalue("operateur")

    
    if operateur == "+":
        resultat = num1 + num2
    elif operateur == "-":
        resultat = num1 - num2
    elif operateur == "*":
        resultat = num1 * num2
    elif operateur == "/":
        if num2 != 0:
            resultat = num1 / num2
        else:
            resultat = "Error : you can't divide by 0 :////"
    else:
        resultat = "Error : invalid operator"
except ValueError:
    resultat = "Error : invalid entry"
except Exception as e:
    resultat = f"Unexpected error : {str(e)}"

print(f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Calculator</title>
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
        <h1>Calculator</h1>
    </div>
    <div class="navbar">
        <ul>
            <li>
                <div class="calculator">
                    <h2 style="text-align: center;">Result :</h2>
                    <div class="result">{resultat}</div>
                    <a href="/html/calculatrice.html" style="text-align: center; display: block;">Return to calculator</a>
                    <a href="/html/index.html" style="text-align: center; display: block;">Home</a>
                </div>
            </li>
        </ul>
    </div>
</body>
</html>""")