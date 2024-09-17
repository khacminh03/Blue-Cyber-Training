<?php
include '../../includes/db.php';
session_start();

if ($_SESSION['role'] != 'student') {
    header("Location: ../login.php");
    exit();
}

$student_id = $_SESSION['user_id'];
$message = '';
$selectedChallenge = null;

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $challenge_id = $_POST['challenge_id'];
    $answer = $_POST['answer'];

    $sql = "SELECT * FROM challenges WHERE id=$challenge_id";
    $result = $conn->query($sql);
    $challenge = $result->fetch_assoc();

    $correct_answer = pathinfo($challenge['file_path'], PATHINFO_FILENAME);

    if ($answer == $correct_answer) {
        $file_path = "../../uploads/challenges/" . $challenge['file_path'];
        $message = "<div class='success'>Correct! The content of the file is:</div>";
        $message .= "<pre>" . htmlspecialchars(file_get_contents($file_path)) . "</pre>";
    } else {
        $message = "<div class='error'>Incorrect answer. Try again.</div>";
    }

    // Lưu thông tin của challenge đã chọn để hiển thị hint và description
    $selectedChallenge = $challenge;
}

$sql = "SELECT * FROM challenges";
$result = $conn->query($sql);

$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Challenge Solver</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            line-height: 1.6;
            margin: 0;
            padding: 20px;
            background-color: #f4f4f4;
            background-image: url('../image/xxx.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            background-attachment: fixed;
        }
        .container {
            max-width: 600px;
            margin: auto;
            background: white;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
        h1 {
            text-align: center;
            color: #333;
        }
        form {
            display: flex;
            flex-direction: column;
        }
        select, input[type="text"], button {
            margin: 10px 0;
            padding: 10px;
            border-radius: 5px;
            border: 1px solid #ddd;
        }
        button {
            background-color: #4CAF50;
            color: white;
            border: none;
            cursor: pointer;
            transition: background-color 0.3s;
        }
        button:hover {
            background-color: #45a049;
        }
        .success {
            color: green;
            font-weight: bold;
        }
        .error {
            color: red;
            font-weight: bold;
        }
        pre {
            background-color: #f8f8f8;
            padding: 10px;
            border-radius: 5px;
            white-space: pre-wrap;
            word-wrap: break-word;
        }
        .hint, .description {
            margin-top: 10px;
            background-color: #f9f9f9;
            padding: 10px;
            border-left: 4px solid #ccc;
            border-radius: 4px;
            font-style: italic;
        }
        .home-button {
            display: inline-block;
            padding: 10px 20px;
            background-color: #3498db;
            color: white;
            text-align: center;
            text-decoration: none;
            border-radius: 5px;
            margin-top: 20px;
            transition: background-color 0.3s;
            font-size: 14px;
        }
        .home-button:hover {
            background-color: #2980b9;
        }
        .button-container {
            text-align: center;
            margin-top: 20px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Challenge Solver</h1>
        <form method="POST" action="">
            <select name="challenge_id" required onchange="this.form.submit()">
                <option value="">Select a Challenge</option>
                <?php while($row = $result->fetch_assoc()) { ?>
                    <option value="<?php echo $row['id']; ?>" 
                        <?php if ($selectedChallenge && $selectedChallenge['id'] == $row['id']) echo 'selected'; ?>>
                        <?php echo htmlspecialchars($row['hint']); ?>
                    </option>
                <?php } ?>
            </select>

            <?php if ($selectedChallenge): ?>
                <div class="description">
                    <strong>Description:</strong>
                    <?php echo isset($selectedChallenge['description']) ? htmlspecialchars($selectedChallenge['description']) : 'No description available'; ?>
                </div>
                <div class="hint">
                    <strong>Hint:</strong>
                    <?php echo isset($selectedChallenge['hint']) ? htmlspecialchars($selectedChallenge['hint']) : 'No hint available'; ?>
                </div>
            <?php endif; ?>

            <input type="text" name="answer" placeholder="Answer" required>
            <button type="submit">Solve Challenge</button>
        </form>
        <?php echo $message; ?>
        

        
    </div>
    <div class="button-container">
            <a href="./dashboard.php" class="home-button">Trang chủ</a>
        </div>
</body>
</html>