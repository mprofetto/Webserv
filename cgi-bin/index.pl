#!/usr/bin/perl

use strict;
use warnings;

# Print the HTML content
print <<"END_HTML";
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sample HTML Page</title>
    <style>
        p {
            color: blue;
            font-size: 16px;
        }
    </style>
</head>
<body>
    <h1>Hello, World!</h1>
    <p>This is a sample HTML page generated using Perl.</p>
    <button onclick="location.href='../cgi-bin/infinite.pl'">Go infinite</button>
    <button onclick="location.href='/index.html'">Go back</button>
</body>
</html>
END_HTML