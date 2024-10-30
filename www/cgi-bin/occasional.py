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
