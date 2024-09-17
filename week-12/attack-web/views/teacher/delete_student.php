<?php
include '../../includes/db.php';
session_start();

if ($_SESSION['role'] != 'teacher') {
    header("Location: ../login.php");
    exit();
}

$student_id = $_GET['id'];

$sql = "DELETE FROM users WHERE id=$student_id";

if ($conn->query($sql) === TRUE) {
    echo "Student deleted successfully.";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
