<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>Teacher Home</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f5f5f5;
            margin: 20px;
        }

        h1 {
            color: #333;
        }

        form {
            margin-bottom: 20px;
        }

        label {
            display: block;
            margin-bottom: 5px;
        }

        textarea {
            width: 100%;
            padding: 5px;
        }

        button[type="submit"] {
            padding: 8px 16px;
            background-color: #4caf50;
            color: #fff;
            border: none;
            cursor: pointer;
        }

        button[type="submit"]:hover {
            background-color: #45a049;
        }

        a {
            color: #0066cc;
            text-decoration: none;
        }
    </style>
</head>

<body>
    <h1>Welcome, <?php session_start(); echo $_SESSION['username']; ?></h1>

    <form action="upload_challenge.php" method="post" enctype="multipart/form-data">
        <!-- Challenge Name -->
        <h2>Challenge Name:</h2>
        <input type="text" name="challenge_name" placeholder="Enter challenge name">

        <!-- Add answer file -->
        <h2><label for="file">Add answer file:</label></h2>
        <input type="file" name="file">
        <br>

        <!-- Task Description -->
        <h2>Task Description:</h2>
        <textarea name="description" rows="4" cols="50" placeholder="Enter task description"></textarea>

        <!-- Task Answer -->
        <h2>Task Answer:</h2>
        <textarea name="answer" rows="4" cols="50" placeholder="Enter task answer"></textarea>

        <!-- Task Deadline -->
        <h2>Task Deadline:</h2>
        <input type="datetime-local" name="deadline">

        <!-- Submit button -->
        <button type="submit" name="submit">Submit</button>
    </form>

    <!-- Logout link -->
    <a href="logout.php">Logout</a>
</body>

</html>