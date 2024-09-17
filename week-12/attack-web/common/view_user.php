<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

$user_id = $_GET['id'];
$sql = "SELECT * FROM users WHERE id=$user_id";
$result = $conn->query($sql);
$user = $result->fetch_assoc();

$conn->close();
?>

<h2><?php echo $user['full_name']; ?></h2>
<p>Username: <?php echo $user['username']; ?></p>
<p>Email: <?php echo $user['email']; ?></p>
<p>Phone Number: <?php echo $user['phone_number']; ?></p>

<a href="send_message.php?receiver_id=<?php echo $user['id']; ?>">Send Message</a>
