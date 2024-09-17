<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

$message_id = $_GET['id'];

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $message_content = $_POST['message_content'];

    $sql = "UPDATE messages SET message_content='$message_content' WHERE id=$message_id";

    if ($conn->query($sql) === TRUE) {
        echo "Message updated successfully.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

$sql = "SELECT * FROM messages WHERE id=$message_id";
$result = $conn->query($sql);
$message = $result->fetch_assoc();

$conn->close();
?>

<form method="POST" action="">
    <textarea name="message_content" required><?php echo $message['message_content']; ?></textarea>
    <button type="submit">Update Message</button>
</form>
