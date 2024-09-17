<?php
include '../../includes/db.php';
session_start();

if ($_SESSION['role'] != 'student') {
    header("Location: ../login.php");
    exit();
}

$student_id = $_SESSION['user_id'];
$message = '';

if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["submission"])) {
    $assignment_id = $_POST['assignment_id'];
    $file_name = $_FILES["submission"]["name"];
    $file_tmp = $_FILES["submission"]["tmp_name"];
    $file_path = "../../uploads/submissions/" . $file_name;

    if (move_uploaded_file($file_tmp, $file_path)) {
        $sql = "INSERT INTO submissions (assignment_id, student_id, file_path) 
                VALUES ($assignment_id, $student_id, '$file_name')";

        if ($conn->query($sql) === TRUE) {
            $message = "<div class='alert alert-success'>Submission uploaded successfully.</div>";
        } else {
            $message = "<div class='alert alert-danger'>Error: " . $conn->error . "</div>";
        }
    } else {
        $message = "<div class='alert alert-danger'>Failed to upload file.</div>";
    }
}

$sql = "SELECT * FROM assignments";
$result = $conn->query($sql);

$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Submit Assignment</title>
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
            max-width: 400px;
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
        select, input[type="file"] {
            margin-bottom: 15px;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 16px;
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
        .download-select {
            margin-top: 10px;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 16px;
        }
        .download-button {
            margin-top: 10px;
            background-color: #3498db;
            color: white;
            padding: 10px;
            border-radius: 4px;
            cursor: pointer;
            text-align: center;
            text-decoration: none;
        }
        .download-button:hover {
            background-color: #2980b9;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>Submit Assignment</h2>
        <?php echo $message; ?>
        <form method="POST" enctype="multipart/form-data" action="">
            <select name="assignment_id" required>
                <option value="">Select Assignment</option>
                <?php while($row = $result->fetch_assoc()) { ?>
                    <option value="<?php echo $row['id']; ?>"><?php echo htmlspecialchars($row['title']); ?></option>
                <?php } ?>
            </select>
            <input type="file" name="submission" required>
            <button type="submit">Submit Assignment</button>
        </form>

        <form method="GET" action="">
            <select name="download_assignment" class="download-select" required>
                <option value="">Download Assignment</option>
                <?php 
                // Reset the result pointer to the start
                $result->data_seek(0); 

                // Loop through assignments to add download options
                while($row = $result->fetch_assoc()) { 
                    $file_path = "../../uploads/assignments/" . $row['file_path'];
                    ?>
                    <option value="<?php echo $file_path; ?>"><?php echo htmlspecialchars($row['title']); ?></option>
                <?php } ?>
            </select>
            <button type="submit" class="download-button">Download</button>
        </form>
    </div>
    <a href="./dashboard.php" class="home-button">Trang chủ</a>
</body>
</html>

<?php
if ($_SERVER["REQUEST_METHOD"] == "GET" && isset($_GET["download_assignment"])) {
    $file_path = $_GET["download_assignment"];
    if (file_exists($file_path)) {
        header('Content-Description: File Transfer');
        header('Content-Type: application/octet-stream');
        header('Content-Disposition: attachment; filename="' . basename($file_path) . '"');
        header('Expires: 0');
        header('Cache-Control: must-revalidate');
        header('Pragma: public');
        header('Content-Length: ' . filesize($file_path));
        flush(); // Flush system output buffer
        readfile($file_path);
        exit();
    } else {
        echo "<div class='alert alert-danger'>File không tồn tại.</div>";
    }
}
?>
