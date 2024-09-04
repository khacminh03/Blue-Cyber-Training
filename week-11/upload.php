<?php
$targetDir = "uploads/";
$fileToUpload = $_FILES["fileToUpload"] ?? null;

if ($fileToUpload && $fileToUpload["error"] == UPLOAD_ERR_OK) {
    $targetFile = $targetDir . basename($fileToUpload["name"]);
    $uploadOk = 1;
    $fileType = strtolower(pathinfo($targetFile, PATHINFO_EXTENSION));

    if (file_exists($targetFile)) {
        echo "File đã tồn tại.";
        $uploadOk = 0;
    }

    if ($fileToUpload["size"] > 500000) {
        echo "File quá lớn.";
        $uploadOk = 0;
    }

    if ($fileType != "txt") {
        echo "Chỉ được phép tải lên các tệp tin .txt.";
        $uploadOk = 0;
    }

    if ($uploadOk == 0) {
        echo "Tệp tin không được tải lên.";
    } else {
        if (move_uploaded_file($fileToUpload["tmp_name"], $targetFile)) {
            echo "Tệp tin " . basename($fileToUpload["name"]) . " đã được tải lên thành công.";
        } else {
            echo "Có lỗi xảy ra khi tải lên tệp tin.";
        }
    }
} else {
    echo "Có lỗi xảy ra khi tải lên tệp tin.";
}

$targetDir = "uploads/";
$fileToUpload = $_FILES["fileToUpload"] ?? null;

if ($fileToUpload && $fileToUpload["error"] == UPLOAD_ERR_OK) {
    $targetFile = $targetDir . basename($fileToUpload["name"]);

    if (move_uploaded_file($fileToUpload["tmp_name"], $targetFile)) {
        echo "Tệp tin " . basename($fileToUpload["name"]) . " đã được tải lên thành công.";
    } else {
        //echo "Có lỗi xảy ra khi tải lên tệp tin.";
    }
} else {
    echo "Không có file đã tải lên.";
}
?>
<h2>Bài tập</h2>
<?php
    $targetDir = "uploads/";
    $files = scandir($targetDir);

    if ($files !== false) {
        echo "<ul>";
        foreach ($files as $file) {
            if ($file != '.' && $file != '..') {
                $filePath = $targetDir . $file;
                echo "<li><a href='view.php?file=$filePath'>$file</a></li>";
            }
        }
        echo "</ul>";
    } else {
        echo "Không có file nào trong thư mục.";
    }

        if (isset($_GET['file'])) {
            $filePath = $_GET['file'];

        if (file_exists($filePath)) {
            $fileContent = file_get_contents($filePath);
                echo "<pre>" . htmlspecialchars($fileContent) . "</pre>";
        } else {
                echo "File không tồn tại.";
        }   
}

if (isset($_GET['file'])) {
    $filePath = $_GET['file'];

    if (file_exists($filePath)) {
        $fileContent = file_get_contents($filePath);
        echo "<h2>Nội dung của file:</h2>";
        echo "<pre>" . htmlspecialchars($fileContent) . "</pre>";
    } else {
        echo "File không tồn tại.";
    }
}
?>
