<?php
include 'includes/db.php';

$password1 = password_hash('123456a@A', PASSWORD_DEFAULT);
$password2 = password_hash('123456a@A', PASSWORD_DEFAULT);

$sql = "INSERT INTO users (username, password, full_name, email, phone_number, role) VALUES 
       ('teacher1', '$password1', 'Teacher One', 'teacher1@example.com', '0123456789', 'teacher'),
       ('teacher2', '$password2', 'Teacher Two', 'teacher2@example.com', '0987654321', 'teacher')";

if ($conn->query($sql) === TRUE) {
    echo "Teachers created successfully.";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
