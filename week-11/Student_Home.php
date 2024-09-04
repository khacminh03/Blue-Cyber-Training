<?php
    session_start();
    require './functions.php';
    connectDB();

    $get_student_data = "SELECT * FROM users WHERE username=?";
    $preparedStatement = $conn->prepare($get_student_data);
    $preparedStatement -> bind_param("s", $id );
    $preparedStatement->execute();
    $result = $preparedStatement->get_result();
?>
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Student Page</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #f2f2f2;
            padding: 30px;
            color: #333;
        }

        h1 {
            text-align: center;
            margin-bottom: 40px;
        }

        h2 {
            margin-top: 40px;
            margin-bottom: 20px;
        }

        form {
            margin-bottom: 30px;
        }

        form a {
            display: inline-block;
            background-color: #4285F4;
            color: #fff;
            padding: 8px 12px;
            text-decoration: none;
            border-radius: 4px;
            transition: background-color 0.3s ease;
            margin-right: 10px;
        }

        form a:hover {
            background-color: #4285F4;
        }

        input[type="file"] {
            margin-top: 10px;
        }

        body {
            background-color: #fafafa;
        }
        .form-group {
            margin-bottom: 20px;
        }

        label {
            display: block;
            margin-bottom: 5px;
            color: #333;
        }

        input[type="text"],
        input[type="file"] {
            width: 100%;
            padding: 8px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }

        input[type="submit"] {
            background-color: #4285F4;
            color: #fff;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }

        input[type="submit"]:hover {
            background-color: #4285F4;
        }
    </style>
</head>

<body>
    <h1>Welcome, Student</h1>

    <!-- Display All Students -->
    <h2>Display Student List</h2>
    <form method="post" action="">
        <a href="Student_List_For_Student.php">Student List</a>
    </form>

    <!-- Update Profile -->
    <h2>Update Profile</h2>
    <form method="post" action="">
        <a href="Student_Edit_For_Student.php">Click here</a>
    </form>
    <h2>Challenge</h2>
    <form method="post" action="">
        <a href="studentHome.php">Click here</a>
    </form>
    <h2>Homework</h2>
    <form method="post" action="">
        <a href="view_student.php">Click here</a>
    </form>
</body>

</html>
