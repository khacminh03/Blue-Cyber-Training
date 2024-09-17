<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

$message_id = $_GET['id'];

$sql = "DELETE FROM messages WHERE id=$message_id";

if ($conn->query($sql) === TRUE) {
    echo "Message deleted successfully.";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
