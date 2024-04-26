#!/usr/bin/perl

use strict;
use warnings;
use CGI;

# Set upload directory path
my $upload_dir = "../upload";

# Create CGI object
my $cgi = CGI->new();

# Check if form was submitted
if ($cgi->request_method() eq 'POST') {
    # Get uploaded file handle
    my $filehandle = $cgi->upload('file');

    # Check if file was uploaded successfully
    if ($filehandle) {
        # Extract filename
        my $filename = $cgi->param('file');
        $filename =~ s/^.*(\\|\/)//;  # Remove any path information from the filename for security

        # Open output file
        open my $outfile, '>', "$upload_dir/$filename" or die "Error opening file: $!";

        # Write uploaded file content to output file
        while (my $bytesread = read($filehandle, my $buffer, 1024)) {
            print $outfile $buffer;
        }

        # Close output file
        close $outfile;

        print "File uploaded successfully!";
    } else {
        print $cgi->header(-status => '400 Bad Request');
        print "Error uploading file!";
    }
} else {
    # Display HTML form
    print <<"HTML";
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>File Upload</title>
    </head>
    <body>
        <h2>Upload a File</h2>
        <form enctype="multipart/form-data" action="upload.pl" method="post">
            <label for="file">Select file:</label>
            <input type="file" id="file" name="file" required><br><br>
            <input type="submit" value="Upload">
            <button onclick="location.href='index.pl'">Go back</button>
        </form>
    </body>
    </html>
HTML
}
