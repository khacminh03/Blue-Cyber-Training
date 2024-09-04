<?php
    session_start();
    function validateDataNew($data) {
        $newData = stripslashes(trim(htmlspecialchars($data, ENT_QUOTES, "UTF-8")));
        return $newData;
    }

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        // Kết nối cơ sở dữ liệu
        $DATABASE_HOST = 'localhost';
        $DATABASE_USER = 'root';
        $DATABASE_PASS = '';
        $DATABASE_NAME = 'code_and_punch';
    
        $con = mysqli_connect($DATABASE_HOST, $DATABASE_USER, $DATABASE_PASS, $DATABASE_NAME);
    
        $studentAnswer = validateDataNew($_POST['answer']);
    
        $query = 'SELECT challenge_name, description, answer, deadline, file_path FROM file WHERE challenge_name = ?';
    
        if ($statement = $con->prepare($query)) {
            $challengeName = $_SESSION['challenge_name'];
            $statement->bind_param('s', $challengeName);
            $statement->execute();
            $statement->bind_result($challengeNameData, $descriptionData, $answerData, $deadlineData, $file_pathData);
    
            // Lấy dữ liệu từ kết quả truy vấn
            if ($statement->fetch()) {
                // Gán giá trị vào các biến
                $challengeNameNew = validateDataNew($challengeNameData);
                $descriptionNew = validateDataNew($descriptionData);
                $answerNew = validateDataNew($answerData);
                $deadlineNew = validateDataNew($deadlineData);
                $file_pathNew = $file_pathData;
                $_SESSION['challenge_name'] = $challengeNameNew;
                $_SESSION['deadline'] = $deadlineNew;
                $_SESSION['answer'] = $answerNew;
                $_SESSION['file_path'] = $file_pathNew;
                $_SESSION['description'] = $descriptionNew;
            }
                
        }
        $con->close();
        


    }
?>