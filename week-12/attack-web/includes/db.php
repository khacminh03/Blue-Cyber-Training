<?php
$servername = "localhost";  // MySQL hostname
$username = "root";               // MySQL username
$password = "";               // MySQL password
$dbname = "school_management"; // MySQL database name

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

mysqli_set_charset($conn, "utf8"); // Set the character set to UTF-8
?>
