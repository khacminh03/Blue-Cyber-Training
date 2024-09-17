<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Đăng ký</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 0;
            min-height: 100vh;
            background-image: url('./image/stude.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            background-attachment: fixed;
        }
        .top-bar {
            background-color: #3498db;
            padding: 10px 20px;
            text-align: left;
        }
        .home-button {
            color: white;
            text-decoration: none;
            font-weight: bold;
            font-size: 16px;
        }
        .home-button:hover {
            text-decoration: underline;
        }
        .container {
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        .form-container {
            background-color: white;
            padding: 30px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            width: 100%;
            max-width: 400px;
        }
        h2 {
            text-align: center;
            color: #333;
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
        form {
            display: flex;
            flex-direction: column;
        }
        label {
            margin-top: 10px;
            font-weight: bold;
            color: #555;
        }
        input, select {
            width: 100%;
            padding: 8px;
            margin-top: 5px;
            margin-bottom: 15px;
            border: 1px solid #ddd;
            border-radius: 4px;
            box-sizing: border-box;
        }
        input[type="submit"] {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 10px;
            cursor: pointer;
            font-size: 16px;
        }
        input[type="submit"]:hover {
            background-color: #45a049;
        }
        .message {
            margin-top: 15px;
            padding: 10px;
            border-radius: 4px;
            text-align: center;
        }
        .success {
            background-color: #d4edda;
            color: #155724;
        }
        .error {
            background-color: #f8d7da;
            color: #721c24;
        }
        
    </style>
</head>
<body>
    <div class="top-bar">
        <a href="index.php" class="home-button">Trang chủ</a>
    </div>
    <div class="container">
        <div class="form-container">
            <h2>Đăng ký tài khoản</h2>
            <form action="register.php" method="POST">
                <label for="username">Username:</label>
                <input type="text" id="username" name="username" required>

                <label for="password">Password:</label>
                <input type="password" id="password" name="password" required>

                <label for="full_name">Full Name:</label>
                <input type="text" id="full_name" name="full_name" required>

                <label for="email">Email:</label>
                <input type="email" id="email" name="email">

                <label for="phone_number">Phone Number:</label>
                <input type="text" id="phone_number" name="phone_number">

                <label for="role">Role:</label>
                <select id="role" name="role" required>
                    <option value="student">Student</option>
                    <option value="teacher">Teacher</option>
                </select>

                <input type="submit" value="Register">
            </form>

            <?php
            if ($_SERVER['REQUEST_METHOD'] == 'POST') {
                $username = $_POST['username'];
                $password = password_hash($_POST['password'], PASSWORD_DEFAULT);
                $full_name = $_POST['full_name'];
                $email = $_POST['email'];
                $phone_number = $_POST['phone_number'];
                $role = $_POST['role'];

                $conn = new mysqli("localhost", "root", "", "chal11");

                if ($conn->connect_error) {
                    die("<div class='message error'>Connection failed: " . $conn->connect_error . "</div>");
                }

                $sql = "INSERT INTO users (username, password, full_name, email, phone_number, role) 
                        VALUES (?, ?, ?, ?, ?, ?)";
                
                $stmt = $conn->prepare($sql);
                $stmt->bind_param("ssssss", $username, $password, $full_name, $email, $phone_number, $role);

                if ($stmt->execute()) {
                    echo "<div class='message success'>User registered successfully</div>";
                } else {
                    echo "<div class='message error'>Error: " . $stmt->error . "</div>";
                }

                $stmt->close();
                $conn->close();
            }
            ?>
        </div>
    </div>
    <footer>
        <p>&copy; 2024 Quản lý sinh viên an toàn</p>
    </footer>
</body>
</html>