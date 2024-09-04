<?php
if (isset($_GET['file'])) {
    $filePath = $_GET['file'];

    if (file_exists($filePath)) {
        $fileContent = file_get_contents($filePath);
        echo "<pre>" . htmlspecialchars($fileContent) . "</pre>";
    } else {
        echo "File không tồn tại.";
    }
} else {
    echo "Chọn một file để xem.";
}
?>  