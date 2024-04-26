#!/usr/bin/python3

import cgi
import os

# Function to create the file with the desired content
def create_file(int1, int2):
    filename = "cgi_{}_{}".format(int1, int2)
    with open("../upload/" + filename, "w") as file:
        file.write("42")
    return filename

# Create an instance of FieldStorage
form = cgi.FieldStorage()

# Print the Content-type header and the start of the HTML response
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>CGI Form Example</title>")
print("</head>")
print("<body>")
print("<h2>Enter Two Integers</h2>")
# Print the HTML form
print("<form method='post' action='{}'>".format(os.environ['SCRIPT_NAME']))
print("Integer 1: <input type='text' name='int1'><br>")
print("Integer 2: <input type='text' name='int2'><br>")
print("<input type='submit' value='Submit'>")
print("</form>")
print("""<button onclick="location.href='index.py'">Go back</button>""")

# Check if the form has been submitted
if form:
    # Get the values of the 'int1' and 'int2' parameters from the form
    int1 = form.getvalue('int1')
    int2 = form.getvalue('int2')
    # Check if both parameters are provided and are integers
    if int1 is not None and int2 is not None:
        try:
            int1 = int(int1)
            int2 = int(int2)
            # Call the function to create the file
            filename = create_file(int1, int2)
            print("<p>File created successfully!</p>")
            print("<p>File name: {}</p>".format(filename))
        except (TypeError, ValueError):
            int1 = 0
            int1 = 0
            filename = create_file(int1, int2)
            print("<p>File created successfully!</p>")
            print("<p>File name: {}</p>".format(filename))
    else:
        print("<p>Error: Both parameters 'int1' and 'int2' are required.</p>")

# Close the HTML response
print("</body>")
print("</html>")
