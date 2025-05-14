<?php
require_once('config.php');
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

// Create connection
$conn = new mysqli(DB_SERVER, DB_USERNAME, DB_PASSWORD, DB_NAME);

// Check connection
if ($conn->connect_error) {
    echo json_encode([
        'status' => 'error',
        'message' => 'Database connection failed',
        'data' => null
    ]);
    exit();
}

// Get latest sensor data
$sql = "SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT 1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    echo json_encode([
        'status' => 'success',
        'data' => [
            'distance' => $row['distance'],
            'acceleration' => [
                'x' => $row['acc_x'],
                'y' => $row['acc_y'],
                'z' => $row['acc_z']
            ],
            'gyroscope' => [
                'x' => $row['gyro_x'],
                'y' => $row['gyro_y'],
                'z' => $row['gyro_z']
            ],
            'timestamp' => $row['timestamp']
        ]
    ]);
} else {
    echo json_encode([
        'status' => 'error',
        'message' => 'No sensor data available',
        'data' => null
    ]);
}

$conn->close();
?>