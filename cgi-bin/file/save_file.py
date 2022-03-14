#!/usr/bin/python

import html
import http.cookies
from json.encoder import ESCAPE_ASCII
import os
import sys
import binascii

input = sys.stdin.readlines()
pos = input[1].find("filename=")
pos += 10
filename = ""
while(input[1][pos] != '\"'):
   filename += input[1][pos]
   pos += 1
pos = filename.find("txt")
if pos != -1:
   del input[0:4]
   list_len = len(input)
   # pos = input[list_len - 1].find("WebKitFormBoundary")
   # if(pos != -1):
   #    del input[list_len -1]
   str = ''.join(input)
else:
   del input[0:4]
   str = ''.join(input)
str = str.encode('utf-8', errors='surrogatepass')
path = "cgi-bin/file/downloads/" + filename #пофиксить пробелы в названии файла
if str:
   try:
      fn = open(path, "wb")
      fn.write(str)
      fn.close()
      message = 'File "' + filename + '" loaded successfully'
   except:
         message = 'Failure, file "' + filename + '" not loaded'
   
else:
   message = 'Failure, file "' + filename + '" not loaded'
print("Content-Type: text/html\n\n")
print("""\
   <!DOCTYPE html>
<html>
<head>
        <meta charset="utf-8" />
        <title>File upload</title>
        <link rel="shortcut icon" href="../favicon.ico" type="image/x-icon">
</head>
<body>
<style>
        body {
                background: url(../../images/background.jpeg);
        }
</style>
<nav class="top-menu">
        <ul class="menu-main">
        <li class="left-item"><a href="https://profile.intra.42.fr/users/cchromos">cchromos</a></li>
        <li class="left-item"><a href="https://profile.intra.42.fr/users/maragorn">maragorn</a></li>
        <li class="left-item"><a href="https://profile.intra.42.fr/users/mstyr">mstyr</a></li>
        <li class="right-item"><a href="../../index.html">Back to the main page</a></li>
        </ul>
</nav>
<link href="../../css/style_user_page.css" rel="stylesheet" type="text/css">
<h3 align="center">%s</h3>
<table align="center">
	<tr>
""" % (message))
print("""\
   <form action = "open_file.py" method = "post">
		<th><p><input class="Open" type="submit" value="Open"></p></th>
      <input type="hidden" value=%s name= "filename">
	</form>
	</tr>
</table>
</body>
</html>
""" % (filename))