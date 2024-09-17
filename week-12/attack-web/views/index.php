<?php
session_start();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quản lý thông tin sinh viên</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            line-height: 1.6;
            margin: 0;
            padding: 0;
            color: #333;
            background-image: url('./image/stude.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            background-attachment: fixed;
            
        }
        /* Thêm một lớp overlay để làm tối ảnh nền và tăng độ tương phản cho nội dung */
        body::before {
            content: '';
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: rgba(0, 0, 0, 0.5); /* Điều chỉnh độ trong suốt ở đây */
            z-index: -1;
        }
        header {
            background-color: rgba(52, 152, 219, 0.8);
            color: #fff;
            padding: 1rem 0;
            text-align: center;
        }
        header h1 {
            margin: 0;
        }
        nav {
            background-color: rgba(41, 128, 185, 0.8);
            padding: 0.5rem;
        }
        nav ul {
            list-style-type: none;
            padding: 0;
            margin: 0;
            display: flex;
            justify-content: center;
        }
        nav ul li {
            margin: 0 10px;
        }
        nav ul li a {
            color: #fff;
            text-decoration: none;
            padding: 5px 10px;
            border-radius: 3px;
            transition: background-color 0.3s;
        }
        nav ul li a:hover {
            background-color: #1abc9c;
        }
        main {
            padding: 2rem;
            max-width: 800px;
            margin: 2rem auto;
            background-color: rgba(255, 255, 255, 0.8);
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            border-radius: 5px;
        }
        h2 {
            color: #2c3e50;
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
</head>
<body>
    <header>
        <h1>Quản lý thông tin sinh viên</h1>
        <nav>
            <ul>
                <li><a href="index.php">Trang chủ</a></li>
                <li><a href="login.php">Đăng nhập</a></li>
            </ul>
        </nav>
    </header>
    <main>
        <h2>Chào mừng đến với hệ thống quản lý sinh viên</h2>
        <p>Ứng dụng giúp bạn quản lý sinh viên, giao bài và nhiều dịch vụ khác.</p>
    </main>
    <footer>
        <p>&copy; 2024 Quản lý sinh viên an toàn</p>
    </footer>
</body>
</html>