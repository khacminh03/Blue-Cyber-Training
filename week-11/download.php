<?php
if (isset($_GET['file_path'])) {
    $file_path = $_GET['file_path'];

    // Kiểm tra xem tệp tin tồn tại hay không
    if (file_exists($file_path)) {
        // Thiết lập tiêu đề tải xuống
        header('Content-Description: File Transfer');
        header('Content-Type: application/octet-stream');
        header('Content-Disposition: attachment; filename=' . basename($file_path));
        header('Content-Transfer-Encoding: binary');
        header('Expires: 0');
        header('Cache-Control: must-revalidate');
        header('Pragma: public');
        header('Content-Length: ' . filesize($file_path));

        // Đọc và gửi nội dung của tệp tin
        ob_clean();
        flush();
        readfile($file_path);
        exit;
    } else {
        echo 'File not found.';
    }
} else {
    echo 'Invalid file path.';
}
?>