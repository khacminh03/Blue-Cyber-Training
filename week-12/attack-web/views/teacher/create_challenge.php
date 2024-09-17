<?php
include '../../includes/db.php';
session_start();

if ($_SESSION['role'] != 'teacher') {
    header("Location: ../login.php");
    exit();
}

$message = '';

if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["challenge_file"])) {
    $hint = $_POST['hint'];
    $file_name = $_FILES["challenge_file"]["name"];
    $file_tmp = $_FILES["challenge_file"]["tmp_name"];
    $file_path = "../../uploads/challenges/" . $file_name;

    if (move_uploaded_file($file_tmp, $file_path)) {
        $teacher_id = $_SESSION['user_id'];
        $sql = "INSERT INTO challenges (teacher_id, hint, file_path) 
                VALUES ($teacher_id, '$hint', '$file_name')";

        if ($conn->query($sql) === TRUE) {
            $message = "<div class='alert alert-success'>Challenge created successfully.</div>";
        } else {
            $message = "<div class='alert alert-danger'>Error: " . $conn->error . "</div>";
        }
    } else {
        $message = "<div class='alert alert-danger'>Failed to upload file.</div>";
    }
}

$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Create Challenge</title>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;500&display=swap" rel="stylesheet">
    <style>
        body {
            font-family: 'Roboto', sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 20px;
            display: flex;
            flex-direction: column;
            align-items: center;
            min-height: 100vh;
            background-image: url('../image/xxx.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            background-attachment: fixed;
        }
        .container {
            background-color: white;
            padding: 30px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            width: 100%;
            max-width: 500px;
            margin-bottom: 20px;
        }
        h2 {
            text-align: center;
            color: #333;
            margin-bottom: 20px;
        }
        form {
            display: flex;
            flex-direction: column;
        }
        textarea {
            margin-bottom: 15px;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 16px;
            height: 100px;
            resize: vertical;
        }
        input[type="file"] {
            margin-bottom: 15px;
        }
        button {
            background-color: #2ecc71;
            color: white;
            border: none;
            padding: 12px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            transition: background-color 0.3s ease;
        }
        button:hover {
            background-color: #27ae60;
        }
        .alert {
            padding: 10px;
            border-radius: 4px;
            margin-bottom: 20px;
            text-align: center;
        }
        .alert-success {
            background-color: #d4edda;
            color: #155724;
        }
        .alert-danger {
            background-color: #f8d7da;
            color: #721c24;
        }
        .home-button {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            text-decoration: none;
            transition: background-color 0.3s ease;
        }
        .home-button:hover {
            background-color: #2980b9;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>Create Challenge</h2>
        <?php echo $message; ?>
        <form method="POST" enctype="multipart/form-data" action="">
            <textarea name="hint" placeholder="Hint" required></textarea>
            <input type="file" name="challenge_file" required>
            <button type="submit">Create Challenge</button>
        </form>
    </div>
    <a href="./dashboard.php" class="home-button">Trang chủ</a>
</body>
</html>