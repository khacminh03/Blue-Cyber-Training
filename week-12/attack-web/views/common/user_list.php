<?php
include '../../includes/db.php';
session_start();

if (!isset($_SESSION['role'])) {
    header("Location: ../login.php");
    exit();
}

$sql = "SELECT * FROM users";
$result = $conn->query($sql);
$dashboard_url = ($_SESSION['role'] == 'teacher') ? '../teacher/dashboard.php' : '../student/dashboard.php';
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>User List</title>
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
            display: flex;
            flex-direction: column;
            align-items: center;
        }
        .container {
            max-width: 800px;
            margin: 0 auto 20px;
            background-color: #fff;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
        h1 {
            color: #333;
            text-align: center;
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
        .view-link {
            color: #007bff;
            text-decoration: none;
        }
        .view-link:hover {
            text-decoration: underline;
        }
        .no-users {
            text-align: center;
            color: #666;
            margin-top: 20px;
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
        <h1>User List</h1>
        <?php if ($result->num_rows > 0): ?>
            <table>
                <tr>
                    <th>Username</th>
                    <th>Full Name</th>
                    <th>Action</th>
                </tr>
                <?php while($row = $result->fetch_assoc()): ?>
                    <tr>
                        <td><?php echo htmlspecialchars($row["username"]); ?></td>
                        <td><?php echo htmlspecialchars($row["full_name"]); ?></td>
                        <td>
                            <a href="view_user.php?id=<?php echo $row["id"]; ?>" class="view-link">View Details</a>
                        </td>
                    </tr>
                <?php endwhile; ?>
            </table>
        <?php else: ?>
            <p class="no-users">No users found.</p>
        <?php endif; ?>
    </div>
    <!-- Sử dụng biến $dashboard_url để điều chỉnh liên kết nút "Trang chủ" -->
    <a href="<?php echo $dashboard_url; ?>" class="home-button">Trang chủ</a>
</body>
</html>

<?php
$conn->close();
?>
