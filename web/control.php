<?php
require_once('config.php');
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    $command = isset($_GET['command']) ? $_GET['command'] : '';
    
    if (!empty($command)) {
        $url = "http://" . ESP32_IP . ":81/control?command=$command";
        
        error_log("Sending command to ESP32: " . $url);
        
        // Send command to ESP32
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_TIMEOUT, 2); // 2 seconds timeout
        
        $response = curl_exec($ch);
        $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        
        curl_close($ch);
        
        if ($http_code == 200) {
            echo json_encode(['status' => 'success', 'message' => 'Command sent successfully']);
        } else {
            $curl_error = curl_error($ch);
            error_log("Failed to send command. Curl error: " . $curl_error);
            echo json_encode(['status' => 'error', 'message' => 'Failed to send command: ' . $curl_error]);
        }
    } else {
        echo json_encode(['status' => 'error', 'message' => 'No command provided']);
    }
} else {
    echo json_encode(['status' => 'error', 'message' => 'Invalid request method']);
}
?>