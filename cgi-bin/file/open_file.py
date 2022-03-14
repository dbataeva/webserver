#!/usr/bin/python

import cgi
import html
import http.cookies
from json.encoder import ESCAPE_ASCII
import os
import sys
import binascii

for line in sys.stdin:
    str = line
data = ""
list = str.split('=')
filename = list[1]
file = ''.join(filename)
path = "cgi-bin/file/downloads/" + file
if str:
   try:
      fn = open(path, "r")
      out = fn.readlines()
      data = ''.join(out)
      fn.close()
      message = 'File "' + file + '" open'
   except:
         message = 'Failure, file "' + file + '" not open'
   
else:
   message = 'Failure, file "' + file + '" not loaded'
print("Content-Type: text/html\n\n")
print ("""\
<!DOCTYPE HTML>
</html>
<head>
	<meta charset=\"utf-8\" />
   <h3 align="center">%s</h3>
	</head>
""" % (message))
print ("""\
<body>
   <p>%s</p>
</body>
</html>
""" % (data))
