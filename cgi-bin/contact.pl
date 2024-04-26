#!/usr/bin/perl

use strict;
use warnings;

# Set upload directory path
my $upload_dir = "../upload";

# Define the CSV file path
my $csv_file = "$upload_dir/contact.csv";

# Get the request body from STDIN if available
my $request_body;
if (defined $ENV{'CONTENT_LENGTH'} && $ENV{'CONTENT_LENGTH'} > 0) {
    read(STDIN, $request_body, $ENV{'CONTENT_LENGTH'});
}

# If request body exists, parse it
if ($request_body) {
    # Parse request body to extract name and email
    my ($name, $email) = split(/&/, $request_body);
    $name =~ s/name=//;
    $email =~ s/email=//;

    # URL decode name and email
    $name =~ s/%([A-Fa-f0-9]{2})/chr(hex($1))/eg;
    $email =~ s/%([A-Fa-f0-9]{2})/chr(hex($1))/eg;

    # Remove leading and trailing whitespace from name and email
    $name =~ s/^\s+|\s+$//g;
    $email =~ s/^\s+|\s+$//g;

    # If name is empty or contains only whitespace, set it to 'Unknown'
    $name = 'Unknown' if $name eq '';

    # Append name and email to the CSV file
    open(my $fh, '>>', $csv_file) or die "Could not open file '$csv_file' $!";
    print $fh "$name,$email\n";
    close $fh;

    # Print HTML response
    print <<"HTML";

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Form Submission</title>
</head>
<body>
    <h2>Form Submission</h2>
    <p>Thank you for submitting the form!</p>
    <button onclick="location.href='index.pl'">Go back</button>
</body>
</html>
HTML
} else {
    # Print HTML form
    print <<"HTML";

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Contact Form</title>
</head>
<body>
    <h2>Contact Form</h2>
    <form action="contact.pl" method="post">
        <label for="name">Name:</label>
        <input type="text" id="name" name="name" required><br><br>
        <label for="email">Email:</label>
        <input type="email" id="email" name="email" required><br><br>
        <input type="submit" value="Submit">
        <button onclick="location.href='index.pl'">Go back</button>
    </form>
</body>
</html>
HTML
}
