<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

// Kiểm tra xem 'id' có được truyền qua URL không
if (!isset($_GET['id'])) {
    echo "No message ID provided. Check the URL for 'id' parameter.";
    exit();
}
$message_id = $_GET['id'];

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $message_content = $_POST['message_content'];

    // Sử dụng prepared statement để tránh SQL injection
    $sql = "UPDATE messages SET message_content = ? WHERE id = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("si", $message_content, $message_id);

    if ($stmt->execute()) {
        echo "Message updated successfully.";
    } else {
        echo "Error: " . $stmt->error;
    }
    $stmt->close();
}

$sql = "SELECT * FROM messages WHERE id = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $message_id);
$stmt->execute();
$result = $stmt->get_result();
$message = $result->fetch_assoc();
$stmt->close();

// Kiểm tra xem tin nhắn có tồn tại không
if (!$message) {
    echo "Message not found.";
    exit();
}
// Kiểm tra xem người dùng có quyền chỉnh sửa tin nhắn này không
if ($message['sender_id'] != $_SESSION['user_id']) {
    echo "You don't have permission to edit this message.";
    exit();
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Edit Message</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <style>
        body {
            background-color: #f8f9fa;
            padding: 20px;
            background-image: url('../image/xxx.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            background-attachment: fixed;
        }
        .container {
            max-width: 600px;
            background-color: #ffffff;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
    </style>
</head>
<body>
    <div class="container">
        <h2 class="mb-4">Edit Message</h2>
        <form method="POST" action="">
            <div class="form-group">
                <textarea name="message_content" class="form-control" rows="5" required><?php echo htmlspecialchars($message['message_content']); ?></textarea>
            </div>
            <button type="submit" class="btn btn-primary">Update Message</button>
        </form>
    </div>
</body>
</html>
