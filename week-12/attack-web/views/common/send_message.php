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
            VALUES (?, ?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("iis", $sender_id, $receiver_id, $message_content);

    if ($stmt->execute() === TRUE) {
        echo "<div class='alert alert-success'>Message sent successfully.</div>";
    } else {
        echo "<div class='alert alert-danger'>Error: " . $stmt->error . "</div>";
    }
}

$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Send Message</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <style>
        body {
            background-color: #f8f9fa;
            background-image: url('../image/xxx.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            background-attachment: fixed;
        }
        .container {
            background-color: #ffffff;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            padding: 30px;
            margin-top: 50px;
        }
        textarea {
            resize: none;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>Send Message</h2>
        <form method="POST" action="">
            <div class="form-group">
                <label for="message_content">Your message:</label>
                <textarea id="message_content" name="message_content" class="form-control" rows="5" placeholder="Write your message here..." required></textarea>
            </div>
            <button type="submit" class="btn btn-primary">Send Message</button>
        </form>
    </div>
</body>
</html>
