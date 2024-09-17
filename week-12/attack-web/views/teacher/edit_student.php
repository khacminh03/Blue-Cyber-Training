<?php
include '../../includes/db.php';
session_start();

if ($_SESSION['role'] != 'teacher') {
    header("Location: ../login.php");
    exit();
}

$student_id = $_GET['id'];

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $password = $_POST['password'];
    $email = $_POST['email'];
    $phone_number = $_POST['phone_number'];

    $sql = "UPDATE users SET password='$password', email='$email', phone_number='$phone_number' WHERE id=$student_id";

    if ($conn->query($sql) === TRUE) {
        echo "Student information updated successfully.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

$sql = "SELECT * FROM users WHERE id=$student_id";
$result = $conn->query($sql);
$student = $result->fetch_assoc();

$conn->close();
?>

<form method="POST" action="">
    <input type="password" name="password" value="<?php echo $student['password']; ?>" required>
    <input type="email" name="email" value="<?php echo $student['email']; ?>" required>
    <input type="text" name="phone_number" value="<?php echo $student['phone_number']; ?>" required>
    <button type="submit">Update Student</button>
</form>
