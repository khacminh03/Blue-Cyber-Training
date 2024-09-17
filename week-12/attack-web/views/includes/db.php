<?php
$servername = "sql110.infinityfree.com";  // MySQL hostname
$username = "if0_37141571";               // MySQL username
$password = "sFcDqPhz7QX5";               // MySQL password
$dbname = "if0_37141571_school_management"; // MySQL database name

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

mysqli_set_charset($conn, "utf8"); // Set the character set to UTF-8
?>
