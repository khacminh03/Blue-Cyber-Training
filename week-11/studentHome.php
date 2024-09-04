<?php
session_start();

function validateDataNew($data) {
    $newData = stripslashes(trim(htmlspecialchars($data, ENT_QUOTES, "UTF-8")));
    return $newData;
}

// connect
$DATABASE_HOST = 'localhost';
$DATABASE_USER = 'root';
$DATABASE_PASS = '';
$DATABASE_NAME = 'CodeAndPunch';

$con = mysqli_connect($DATABASE_HOST, $DATABASE_USER, $DATABASE_PASS, $DATABASE_NAME);

$query = 'SELECT challenge_name, description, answer, deadline, file_path FROM challenge';

if ($result = mysqli_query($con, $query)) {
    $challenges = array();

    while ($row = mysqli_fetch_assoc($result)) {
        $challengeNameData = $row['challenge_name'];
        $descriptionData = $row['description'];
        $answerData = $row['answer'];
        $deadlineData = $row['deadline'];
        $file_pathData = $row['file_path'];

        // attach variable
        $challengeNameNew = validateDataNew($challengeNameData);
        $descriptionNew = validateDataNew($descriptionData);
        $answerNew = validateDataNew($answerData);
        $deadlineNew = validateDataNew($deadlineData);
        $file_pathNew = $file_pathData;

        // more information
        $challenges[] = array(
            'challenge_name' => $challengeNameNew,
            'deadline' => $deadlineNew,
            'answer' => $answerNew,
            'file_path' => $file_pathNew,
            'description' => $descriptionNew
        );
    }

    $_SESSION['challenges'] = $challenges;
}

$con->close();
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>Student Home</title>
</head>

<body>
    <h1>Welcome, <?php echo $_SESSION['username']; ?></h1>

    <?php
    if (isset($_SESSION['challenges']) && !empty($_SESSION['challenges'])) {
        foreach ($_SESSION['challenges'] as $challenge) {
            echo '<h1>Your challenge</h1>';
            echo '<h2>' . $challenge['challenge_name'] . '</h2>';

            echo '<h2 name="deadline">Deadline:</h2>';
            echo isset($challenge['deadline']) ? $challenge['deadline'] : 'Nothing here';

            echo '<h2 name="description">Description:</h2>';
            echo isset($challenge['description']) ? $challenge['description'] : 'Nothing here';

            echo '<h2>Write your answer:</h2>';
            echo '<form method="post">';
            echo '<textarea name="answer" rows="4" cols="50" placeholder="Enter your answer"></textarea>';
            echo '<button type="submit" name="submit_answer">Submit Answer</button>';
            echo '</form>';

            try {
                if (isset($challenge['answer']) && $_POST['answer'] == $challenge['answer']) {
                    echo 'Right answer';
                    echo '<h2>Answer file:</h2>';
                    echo '<a href="download.php?file_path=' . $challenge['file_path'] . '">Download answer text here</a>';
                } else {
                    echo 'No answer file available.';
                }
            } catch(Exception $e) {
                  
            }
        }
    } else {
        echo 'No challenges available.';
    }
    ?>

    <!-- Logout link -->
    <a href="logout.php">Logout</a>
</body>

</html>