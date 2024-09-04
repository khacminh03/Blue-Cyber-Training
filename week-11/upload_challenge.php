<?php
    session_start();
    
    function validateData($data) {
        $newData = stripslashes(trim(htmlspecialchars($data, ENT_QUOTES, "UTF-8")));
        return $newData;
    }

    
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {

        //connect database
        $DATABASE_HOST = 'localhost';
        $DATABASE_USER = 'root';
        $DATABASE_PASS = '';
        $DATABASE_NAME = 'CodeAndPunch';

        $con = mysqli_connect($DATABASE_HOST, $DATABASE_USER, $DATABASE_PASS, $DATABASE_NAME);

        $challengeName = validateData($_POST['challenge_name']);
        $description = validateData($_POST['description']);
        $answer = validateData($_POST['answer']);
        $deadline = validateData($_POST['deadline']);

        // get file         
        if($_FILES['file']) {
            try {
                $file_tmp = $_FILES['file']['tmp_name'];
                $file_name = $_FILES['file']['name'];
                $file_type = $_FILES['file']['type'];
                $file_size = $_FILES['file']['size'];
                
                // check file txt or not
                $allowed_file_types = array("text/plain");
                if (!in_array($file_type, $allowed_file_types)) {
                    echo "Wrong type of file.";
                    exit;
                }
                
                // path to save file
                $upload_directory = "D:\CodeAndPunch\CodeAndPunch\challenge\_";
                $file_path = $upload_directory . $file_name;
                
                // move file to final destination
                if (move_uploaded_file($file_tmp, $file_path)) {
                    
                    // query to do it
                    $query = 'INSERT INTO challenge (challenge_name, description, answer, deadline, file_path) VALUES (?, ?, ?, ?, ?)';
                    if ($statement = $con->prepare($query)) {
                        $statement->bind_param('sssss', $challengeName, $description, $answer, $deadline, $file_path);
                        $statement->execute();
                        $statement->close();
                        echo "Upload successfully.";
                    } else {
                        echo "Failed to execute the query.";
                    }
                } else {
                    echo "Failed to upload the file.";
                }

            } catch (Exception $e) {
                echo 'Something went wrong. Contact lead Hoang Quan now.';
            }
        }
        
        $con->close();
    }
?>
