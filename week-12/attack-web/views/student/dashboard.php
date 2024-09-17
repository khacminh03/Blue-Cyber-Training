<?php
session_start();
if ($_SESSION['role'] != 'student') {
    header("Location: ../login.php");
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Student Dashboard</title>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;500&display=swap" rel="stylesheet">
    <style>
        body {
            font-family: 'Roboto', sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 0;
            background-image: url('../image/xxx.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            background-attachment: fixed;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
        }
        header {
            background-color: #2ecc71;
            color: white;
            padding: 20px 0;
            text-align: center;
        }
        h1 {
            margin: 0;
        }
        .dashboard-menu {
            display: flex;
            justify-content: space-around;
            flex-wrap: wrap;
            margin-top: 30px;
        }
        .menu-item {
            background-color: white;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            padding: 20px;
            margin: 10px;
            width: 200px;
            text-align: center;
            transition: transform 0.3s ease, box-shadow 0.3s ease;
        }
        .menu-item:hover {
            transform: translateY(-5px);
            box-shadow: 0 4px 8px rgba(0,0,0,0.2);
        }
        .menu-item a {
            text-decoration: none;
            color: #333;
            font-weight: 500;
            font-size: 18px;
            display: block;
        }
        .menu-item i {
            font-size: 48px;
            margin-bottom: 10px;
            color: #2ecc71;
        }
        footer {
            text-align: center;
            padding: 1rem;
            background-color: rgba(52, 73, 94, 0.8);
            color: #ecf0f1;
            position: fixed;
            bottom: 0;
            width: 100%;
        }
    </style>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css">
</head>
<body>
    <header>
        <div class="container">
            <h1>Student Dashboard</h1>
        </div>
    </header>
    
    <main class="container">
        <div class="dashboard-menu">
            <div class="menu-item">
                <i class="fas fa-edit"></i>
                <a href="edit_profile.php">Edit profile</a>
            </div>
            <div class="menu-item">
                <i class="fas fa-check"></i>
                <a href="submit_assignment.php">submit_assignment</a>
            </div>
            <div class="menu-item">
                <i class="fas fa-heart"></i>
                <a href="solve_challenge.php">Solve challenge</a>
            </div>
            <div class="menu-item">
                <i class="fas fa-users"></i>
                <a href="../common/user_list.php">View User List</a>
            </div>
            <div class="menu-item">
                <i class="fas fa-sign-out-alt"></i>
                <a href="../logout.php">Logout</a>
            </div>
        </div>
    </main>
    <footer>
        <p>&copy; 2024 Quản lý sinh viên an toàn</p>
    </footer>
</body>
</html>

