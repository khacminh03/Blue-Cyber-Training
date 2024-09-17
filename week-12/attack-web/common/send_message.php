<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

$sender_id = $_SESSION['user_id'];
$receiver_id = $_GET['receiver_id'];

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $message_content = $_POST['message_content'];

    $sql = "INSERT INTO messages (sender_id, receiver_id, message_content) 
            VALUES ($sender_id, $receiver_id, '$message_content')";

    if ($conn->query($sql) === TRUE) {
        echo "Message sent successfully.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

$conn->close();
?>

<form method="POST" action="">
    <textarea name="message_content" placeholder="Your message" required></textarea>
    <button type="submit">Send Message</button>
</form>
