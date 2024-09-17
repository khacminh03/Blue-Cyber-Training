<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

if (!isset($_SESSION['user_id'])) {
    echo "User not logged in.";
    exit();
}

$user_id = $_GET['id'];

$sql = "SELECT * FROM users WHERE id = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $user_id);
$stmt->execute();
$result = $stmt->get_result();
$user = $result->fetch_assoc();

$logged_in_user_id = $_SESSION['user_id'];

$sql = "SELECT m.*, 
               CASE 
                   WHEN m.sender_id = ? THEN 'Sent'
                   ELSE 'Received'
               END AS message_type,
               u.full_name AS other_user_name
        FROM messages m 
        JOIN users u ON (m.sender_id = u.id OR m.receiver_id = u.id) AND u.id != ?
        WHERE (m.sender_id = ? AND m.receiver_id = ?) OR (m.sender_id = ? AND m.receiver_id = ?)
        ORDER BY m.sent_at DESC";
$stmt = $conn->prepare($sql);
$stmt->bind_param("iiiiii", $logged_in_user_id, $logged_in_user_id, $logged_in_user_id, $user_id, $user_id, $logged_in_user_id);
$stmt->execute();
$messages = $stmt->get_result();

$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>User Profile</title>
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
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            padding: 30px;
            margin-top: 50px;
        }
        .profile-info {
            margin-bottom: 30px;
        }
        .message-box {
        margin-bottom: 15px;
        padding: 10px;
        border-radius: 5px;
    }
    .message-box.sent {
        background-color: #e3f2fd;
        border-left: 4px solid #2196f3;
    }
    .message-box.received {
        background-color: #f1f8e9;
        border-left: 4px solid #4caf50;
    }
    .message-type {
        font-weight: bold;
        color: #333;
    }
    .message-content {
        margin-top: 5px;
    }
    .message-time {
        font-size: 0.8em;
        color: #666;
    }
    .message-actions {
        margin-top: 10px;
    }
    </style>
</head>
<body>
    <div class="container">
        <div class="profile-info">
            <h2 class="mb-4"><?php echo htmlspecialchars($user['full_name']); ?></h2>
            <p><strong>Username:</strong> <?php echo htmlspecialchars($user['username']); ?></p>
            <p><strong>Email:</strong> <?php echo htmlspecialchars($user['email']); ?></p>
            <p><strong>Phone Number:</strong> <?php echo htmlspecialchars($user['phone_number']); ?></p>
        </div>

        <h3 class="mb-3">Messages</h3>
        <?php if ($messages->num_rows > 0): ?>
    <?php while ($message = $messages->fetch_assoc()): ?>
        <div class="message-box <?php echo $message['message_type'] == 'Sent' ? 'sent' : 'received'; ?>">
            <p class="message-type"><?php echo $message['message_type']; ?> to/from: <?php echo htmlspecialchars($message['other_user_name']); ?></p>
            <p class="message-content"><?php echo htmlspecialchars($message['message_content']); ?></p>
            <?php if (isset($message['sent_at'])): ?>
                <p class="message-time">Sent at: <?php echo htmlspecialchars($message['sent_at']); ?></p>
            <?php endif; ?>
            <?php if ($message['message_type'] == 'Sent'): ?>
                <div class="message-actions">
                    <a href="edit_message.php?id=<?php echo $message['id']; ?>" class="btn btn-sm btn-primary">Edit</a>
                    <a href="delete_message.php?id=<?php echo $message['id']; ?>" class="btn btn-sm btn-danger">Delete</a>
                </div>
            <?php endif; ?>
        </div>
    <?php endwhile; ?>
<?php else: ?>
    <p>No messages between you and this user.</p>
<?php endif; ?>

        <a href="send_message.php?receiver_id=<?php echo $user['id']; ?>" class="btn btn-success mt-3">Send Message</a>
    </div>

    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>