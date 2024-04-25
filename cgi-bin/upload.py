#!/usr/bin/env python3
import cgi
import os

# Set the directory for storing uploaded files
UPLOAD_DIR = '../upload/'

def save_uploaded_file(fileitem):
    # Get the filename
    filename = os.path.basename(fileitem.filename)
    # Create a new file in the upload directory
    with open(os.path.join(UPLOAD_DIR, filename), 'wb') as f:
        # Read and write the file in chunks
        while True:
            chunk = fileitem.file.read(1024)
            if not chunk:
                break
            f.write(chunk)
    return filename

def main():
    print("Content-Type: text/html\n")  # Required header for CGI

    # Check if the form was submitted
    if os.environ['REQUEST_METHOD'] == 'POST':
        # Create instance of FieldStorage
        form = cgi.FieldStorage()

        # Check if the file upload field is in the form
        if 'file' in form:
            fileitem = form['file']
            
            # Check if the file was uploaded successfully
            if fileitem.filename:
                # Save the uploaded file
                filename = save_uploaded_file(fileitem)
                print(f"<p>File '{filename}' uploaded successfully.</p>")
            else:
                print("<p>No file was uploaded.</p>")
        else:
            print("<p>File upload field not found in the form.</p>")
    else:
        # Display the upload form
        print('''
            <form enctype="multipart/form-data" action="upload.py" method="post">
                <input type="file" name="file">
                <input type="submit" value="Upload File">
            </form>
        ''')

if __name__ == '__main__':
    main()
