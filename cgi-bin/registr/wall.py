#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import cgi
import html
import http.cookies
import os
import sys

for line in sys.stdin:
    str = line

from _wall import Wall
wall = Wall()

cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))
session = cookie.get("session")
if session is not None:
    session = session.value
user = wall.find_cookie(session)  # Ищем пользователя по переданной куке

# form = cgi.FieldStorage()
# action = form.getfirst("action", "")
list = str.split('&')
login_list = list[0].split('=')
password_list = list[1].split('=')
action_list = list[2].split('=')
if action_list[1] == "login":
    login = login_list[1]
    login = html.escape(login)
    password = password_list[1]
    password = html.escape(password)
    if wall.find(login, password):
        cookie = wall.set_cookie(login)
        print("Content-Type: text/html")
        print('Set-cookie: session={}'.format(cookie), "\n")
        print ("""\
        <!DOCTYPE html>
        <html>
        <head>
        <meta charset=\"utf-8\" />
        <title>Welcome to our site</title>
        <head>
        <body>
        <style>
        body {
        background: url(../../images/background.jpeg);
        }
        </style>
        <nav class=\"top-menu\">
        <ul class=\"menu-main\">
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/cchromos\">cchromos</a></li>
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/maragorn\">maragorn</a></li>
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/mstyr\">mstyr</a></li>
        <li class=\"right-item\"><a href=\"../../index.html\">Back to the main page</a></li>
        </ul>
        </nav>
        <link href=\"../../css/style_user_page.css\" rel=\"stylesheet\" type=\"text/css\">
        <script>
        document.write (\"<h1 align=center>\"+\"Welcome %s!</h1>\")
        </script>
        </body>
        </html>
        """ %(login))
    # elif wall.find(login):
    #     print("nooooooo" % login)
    #     pass  # А надо бы предупреждение выдать
    else:
        print("Content-Type: text/html")
        print('Set-cookie: session={}'.format(cookie), "\n")
        print ("""\
        <!DOCTYPE html>
        <html>
        <head>
        <meta charset=\"utf-8\" />
        <title>Welcome to our site</title>
        <head>
        <body>
        <style>
        body {
        background: url(../../images/background.jpeg);
        }
        </style>
        <nav class=\"top-menu\">
        <ul class=\"menu-main\">
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/cchromos\">cchromos</a></li>
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/maragorn\">maragorn</a></li>
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/mstyr\">mstyr</a></li>
        <li class=\"right-item\"><a href=\"../../index.html\">Back to the main page</a></li>
        </ul>
        </nav>
        <link href=\"../../css/style_user_page.css\" rel=\"stylesheet\" type=\"text/css\">
        <script>
        document.write (\"<h1 align=center>\"+\"User with login %s not found!</h1>\")
        </script>
        </body>
        </html>
        """ %(login))
elif action_list[1] == "signup":
    login = login_list[1]
    login = html.escape(login)
    password = password_list[1]
    password = html.escape(password)
    if wall.find(login, password):
        cookie = wall.set_cookie(login)
        print("Content-Type: text/html")
        print('Set-cookie: session={}'.format(cookie), "\n")
        print ("""\
        <!DOCTYPE html>
        <html>
        <head>
        <meta charset=\"utf-8\" />
        <title>Welcome to our site</title>
        <head>
        <body>
        <style>
        body {
        background: url(../../images/background.jpeg);
        }
        </style>
        <nav class=\"top-menu\">
        <ul class=\"menu-main\">
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/cchromos\">cchromos</a></li>
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/maragorn\">maragorn</a></li>
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/mstyr\">mstyr</a></li>
        <li class=\"right-item\"><a href=\"../../index.html\">Back to the main page</a></li>
        </ul>
        </nav>
        <link href=\"../../css/style_user_page.css\" rel=\"stylesheet\" type=\"text/css\">
        <script>
        document.write (\"<h1 align=center>\"+\"User with login %s is registered!</h1>\")
        </script>
        </body>
        </html>
        """ %(login))
    # elif wall.find(login):
    #     print("nooooooo" % login)
    else:
        wall.register(login, password)
        cookie = wall.set_cookie(login)
        print("Content-Type: text/html")
        print('Set-cookie: session={}'.format(cookie), "\n")
        print ("""\
        <!DOCTYPE html>
        <html>
        <head>
        <meta charset=\"utf-8\" />
        <title>Welcome to our site</title>
        <head>
        <body>
        <style>
        body {
        background: url(../../images/background.jpeg);
        }
        </style>
        <nav class=\"top-menu\">
        <ul class=\"menu-main\">
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/cchromos\">cchromos</a></li>
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/maragorn\">maragorn</a></li>
        <li class=\"left-item\"><a href=\"https://profile.intra.42.fr/users/mstyr\">mstyr</a></li>
        <li class=\"right-item\"><a href=\"../../index.html\">Back to the main page</a></li>
        </ul>
        </nav>
        <link href=\"../../css/style_user_page.css\" rel=\"stylesheet\" type=\"text/css\">
        <script>
        document.write (\"<h1 align=center>\"+\"Welcome %s!</h1>\")
        </script>
        </body>
        </html>
        """ %(login))
