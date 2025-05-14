<?php
require_once('config.php');

// Prevent caching
header('Cache-Control: no-store, no-cache, must-revalidate, max-age=0');
header('Cache-Control: post-check=0, pre-check=0', false);
header('Pragma: no-cache');
header('Content-Type: image/jpeg');
header('Access-Control-Allow-Origin: *');

$url = "http://" . ESP32_IP . ":80/capture"; // Explicitly use port 80 for video

// Get image from ESP32-CAM
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_TIMEOUT, 3);       // 3 seconds timeout
curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 2); // 2 seconds connection timeout

$image = curl_exec($ch);
$http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
$curl_error = curl_error($ch);

curl_close($ch);

if ($http_code == 200 && !empty($image)) {
    echo $image;
} else {
    error_log("Camera feed error: HTTP $http_code - $curl_error");
    // Return a blank image or error image
    if (file_exists("error.jpg")) {
        readfile("error.jpg");
    } else {
        // Create a simple error image if error.jpg doesn't exist
        $img = imagecreatetruecolor(640, 480);
        $bg = imagecolorallocate($img, 200, 200, 200);
        $text_color = imagecolorallocate($img, 50, 50, 50);
        imagefilledrectangle($img, 0, 0, 640, 480, $bg);
        imagestring($img, 5, 220, 230, "Camera Error", $text_color);
        imagejpeg($img);
        imagedestroy($img);
    }
}
?>