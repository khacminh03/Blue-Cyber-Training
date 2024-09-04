<h1>Bài tập</h1>
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
