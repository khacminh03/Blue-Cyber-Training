<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

$message_id = $_GET['id'];
// Get the user ID of the message receiver or sender (to redirect back to the profile)
$sql = "SELECT receiver_id, sender_id FROM messages WHERE id = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $message_id);
$stmt->execute();
$result = $stmt->get_result();
$message = $result->fetch_assoc();

// Delete the message
$sql = "DELETE FROM messages WHERE id = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $message_id);

if ($stmt->execute()) {
    $redirect_user_id = ($_SESSION['user_id'] == $message['receiver_id']) ? $message['sender_id'] : $message['receiver_id'];
    header("Location: view_user.php?id=" . $redirect_user_id);
} else {
    echo "Error: " . $stmt->error;
}

$conn->close();
?>
