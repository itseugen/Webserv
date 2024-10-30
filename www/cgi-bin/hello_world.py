#!/usr/bin/env python3
import os

request_method = os.environ.get("REQUEST_METHOD", "GET")
if request_method == "GET":
	print("Content-Length: 53\r\n\n")
	print("<html><body><h1>Hello from GET request!</h1></body></html>")
elif request_method == "POST":
	print("Content-Length: 54\r\n\n")
	print("<html><body><h1>Hello from POST request!</h1></body></html>")
else:
	print("Content-Length: 56\r\n\n")
	print("<html><body><h1>Unsupported request method</h1></body></html>")

# !/usr/bin/env python3
# print("HTTP/1.1 200 OK\r\n")
# print("Content-Type: text/html\r\n")
# print("Content-Length: 64\r\n")
# print()  # Blank line separates headers from body
# print("<html><body>")
# print("<h1>Hello, World from Python CGI!</h1>")
# print("</body></html>")

# for i in range(20):
# 	print("<p>MK is here !!</p>")