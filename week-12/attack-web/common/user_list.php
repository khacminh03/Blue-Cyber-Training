<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

$sql = "SELECT * FROM users";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        echo "Username: " . $row["username"] . " - Full Name: " . $row["full_name"] . 
             " - <a href='view_user.php?id=" . $row["id"] . "'>View Details</a><br>";
    }
} else {
    echo "No users found.";
}

$conn->close();
?>
