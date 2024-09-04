<?php 
    require './functions.php';
    $id = isset($_GET['id']) ? (int)$_GET['id'] : '';
    if ($id) {
        $data = get_1_student_infor($id);
    }
    if (!$data) {
        header("Location: Student_List_For_Student.php");
        exit();
    }

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        $data['email'] = isset($_POST['email']) ? $_POST['email'] : '';
        $data['phone_number'] = isset($_POST['phone_number']) ? $_POST['phone_number'] : '';
        $data['password'] = isset($_POST['password']) ? $_POST['password'] : '';
        $data['id'] = isset($_POST['id']) ? $_POST['id'] : '';

        // Validate thông tin
        $errors = array();
        if (empty($data['name'])) {
            $errors['name'] = 'Chưa nhập tên sinh viên';
        }

        if (empty($data['email'])) {
            $errors['email'] = 'Chưa nhập email sinh viên';
        }

        // Nếu không có lỗi thì chỉnh sửa sinh viên
        if (empty($errors)) {
            Edit_Student_For_Student($data['id'], $data['email'], $data['phone_number'], $data['password']);
            // Trở về trang danh sách
            header("Location: Student_List.php");
            exit();
        }
    }

    disconnectDB();
?>
<!DOCTYPE html>
<html>
<head>
    <title>Edit Student</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {
            font-family: Arial, sans-serif;
            padding: 20px;
            background-color: #f2f2f2;
        }

        h1 {
            text-align: center;
            margin-bottom: 30px;
        }

        form {
            max-width: 400px;
            margin: 0 auto;
            background-color: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        label {
            display: block;
            margin-bottom: 8px;
        }

        input[type="text"],
        input[type="password"],
        input[type="email"] {
            width: 100%;
            padding: 10px;
            border: 1px solid #ccc;
            border-radius: 4px;
            font-size: 16px;
        }

        input[type="submit"] {
            display: block;
            width: 100%;
            background-color: #4285F4;
            color: #fff;
            padding: 10px;
            border: none;
            border-radius: 4px;
            font-size: 16px;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }

        input[type="submit"]:hover {
            background-color: #1967D2;
        }

        .error-message {
            color: #FF0000;
            margin-top: 10px;
        }

        .success-message {
            color: #00CC00;
            margin-top: 10px;
        }
    </style>
</head>
<body>
    <h1>Edit Student</h1>
    <a href="Student_List.php">Trở về</a><br><br>
    <form method="post" action="Student_Edit_For_Student.php?id=<?php echo $data['id']; ?>">
        <table border="0" cellspacing="0" cellpadding="10">
            <tr>
                <td>Email</td>
                <td>
                    <input type="text" name="email" value="<?php echo $data['email']; ?>"/>
                    <?php if (!empty($errors['email'])) echo $errors['email']; ?>
                </td>
            </tr>
            <tr>
                <td>Phone number</td>
                <td>
                    <input type="text" name="phone_number" value="<?php echo $data['phone_number']; ?>"/>
                </td>
            </tr>
            <tr>
                <td>Password</td>
                <td>
                    <input type="password" name="password" value="<?php echo $data['password']; ?>"/>
                </td>
            </tr>
            <tr>
                <td></td>
                <td>
                    <input type="hidden" name="id" value="<?php echo $data['id']; ?>"/>
                    <input type="submit" name="Edit_Student_For_Student" value="Lưu"/>
                </td>
            </tr>
        </table>
    </form>
</body>
</html>