#!/usr/bin/env python3

print("""
<!DOCTYPE html>
<html>
<head>
    <title>CGI Python Example</title>
</head>
<body>
    <h1>Hello, CGI Python!</h1>
    <p>This is an example of executing Python script via CGI.</p>
    <button onclick="location.href='../cgi_bin/infinite.py'">Go infinite</button>
    <button onclick="location.href='/index.html'">Go back</button>
</body>
</html>
""")