<?php
include '../../includes/db.php';
session_start();

if ($_SESSION['role'] != 'teacher') {
    header("Location: ../login.php");
    exit();
}

$teacher_id = $_SESSION['user_id'];
$sql = "SELECT s.*, u.full_name, a.title FROM submissions s 
        JOIN users u ON s.student_id = u.id 
        JOIN assignments a ON s.assignment_id = a.id 
        WHERE a.teacher_id = $teacher_id";
$result = $conn->query($sql);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>View Submissions</title>
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
            max-width: 800px;
            margin-bottom: 20px;
        }
        h2 {
            text-align: center;
            color: #333;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th, td {
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }
        th {
            background-color: #f2f2f2;
            font-weight: bold;
        }
        tr:hover {
            background-color: #f5f5f5;
        }
        .download-link {
            color: #3498db;
            text-decoration: none;
        }
        .download-link:hover {
            text-decoration: underline;
        }
        .no-submissions {
            text-align: center;
            color: #777;
            font-style: italic;
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
        <h2>Student Submissions</h2>
        <?php if ($result->num_rows > 0): ?>
            <table>
                <tr>
                    <th>Assignment</th>
                    <th>Student</th>
                    <th>Action</th>
                </tr>
                <?php while($row = $result->fetch_assoc()): ?>
                    <tr>
                        <td><?php echo htmlspecialchars($row["title"]); ?></td>
                        <td><?php echo htmlspecialchars($row["full_name"]); ?></td>
                        <td>
                            <a href="../../uploads/submissions/<?php echo htmlspecialchars($row["file_path"]); ?>" 
                               class="download-link" download>
                                Download Submission
                            </a>
                        </td>
                    </tr>
                <?php endwhile; ?>
            </table>
        <?php else: ?>
            <p class="no-submissions">No submissions found.</p>
        <?php endif; ?>
    </div>
    <a href="./dashboard.php" class="home-button">Trang chủ</a>
</body>
</html>

<?php
$conn->close();
?>