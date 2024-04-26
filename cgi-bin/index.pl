#!/usr/bin/perl

use strict;
use warnings;

print <<HTML;

<!DOCTYPE html>
<html>
<head>
    <title>CGI Perl Example</title>
    <style>
        p {
            color: blue;
            font-size: 16px;
        }
    </style>
</head>
<body>
    <h1>Hello, CGI Perl!</h1>
    <p>This is an example of executing Perl script via CGI.</p>
    <button onclick="location.href='infinite.pl'">Go infinite</button>
    <button onclick="location.href='upload.pl'">Upload a file</button>
    <button onclick="location.href='contact.pl'">Leave your contact</button>
    <button onclick="location.href='/index.html'">Go back</button>
</body>
</html>
HTML
