#!/usr/bin/env python

import cgi
import os

# Set the directory where uploaded files will be stored
UPLOAD_DIR = "/upload"

# HTML template for the upload form
UPLOAD_FORM = """
<!DOCTYPE html>
<html>
<head>
    <title>File Upload</title>
</head>
<body>
    <h2>Upload a File</h2>
    <form method="post" enctype="multipart/form-data">
        <input type="file" name="file" />
        <input type="submit" value="Upload" />
    </form>
</body>
</html>
"""

# Function to handle file uploads
def handle_upload():
    form = cgi.FieldStorage()

    if "file" in form:
        fileitem = form["file"]

        # Check if the file was successfully uploaded
        if fileitem.file:
            # Save the uploaded file to the upload directory
            filename = os.path.join(UPLOAD_DIR, fileitem.filename)
            with open(filename, "wb") as f:
                f.write(fileitem.file.read())

            print("Content-Type: text/html")
            print()
            print("<p>File '{}' uploaded successfully.</p>".format(fileitem.filename))
        else:
            print("Content-Type: text/html")
            print()
            print("<p>No file was uploaded.</p>")
    else:
        print("Content-Type: text/html")
        print()
        print("<p>No file field found in the request.</p>")

# Main function to handle requests
def main():
    print("Content-Type: text/html")
    print()

    if os.environ.get("REQUEST_METHOD", "") == "POST":
        handle_upload()
    else:
        print(UPLOAD_FORM)

if __name__ == "__main__":
    main()
