<?php
    session_start();
    require './functions.php';
    connectDB();

    $get_teacher_data = "SELECT * FROM users WHERE username=?";
    $preparedStatement = $conn->prepare($get_teacher_data);
    $preparedStatement -> bind_param("s", $username );
    $preparedStatement->execute();
    $result = $preparedStatement->get_result();
?>
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Teacher Page</title>
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

        /* Update background color */
        body {
            background-color: #fafafa;
        }

        /* Update form styles */
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
    <h1>Welcome, Teacher</h1>

    <!-- Add Student Form -->
    <h2>Add Student</h2>
    <form method="post" action="">
        <!-- Student fields here -->
        <a href="Add_Student.php">Add Student</a>
    </form>
    <!-- Display All Students -->
    <h2>Display Student List</h2>
    <form method="post" action="">
        <!-- Student fields here -->
        <a href="Student_List.php">Student List</a>
    </form>

    <!-- Upload Homework Form -->
    <h2>Upload Homework</h2>
    <form action="upload.php" method="post" enctype="multipart/form-data">
        Select file to upload:
        <input type="file" name="fileToUpload" id="fileToUpload">
        <input type="submit" value="Upload File" name="submit">
    </form>

    <!-- Upload Challenge Form -->
    <div class="upload-form">
        <h2>Upload Challenge</h2>
        <form method="post" action="">
        <!-- Student fields here -->
        <a href="teacherHome.php">Upload Challenge</a>
    </form>
    </div>
    <!-- View Submitted Homework -->
    <h2>View Submitted Homework</h2>
    <form method="post" action="">
        <!-- Student fields here -->
        <a href="view.php">Click here to view</a>
    </form>

    <!-- Update Profile -->
    <h2>Update Profile</h2>
    <form method="post" action="">
        <!-- Student fields here -->
        <a href="teachers.php">Click here</a>
    </form>
</body>

</html>
