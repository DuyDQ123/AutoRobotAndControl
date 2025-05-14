<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

// Database configuration
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "robot_db";

// Get data from ESP32
$distance = isset($_GET['distance']) ? floatval($_GET['distance']) : null;
$acc_x = isset($_GET['ax']) ? intval($_GET['ax']) : null;
$acc_y = isset($_GET['ay']) ? intval($_GET['ay']) : null;
$acc_z = isset($_GET['az']) ? intval($_GET['az']) : null;
$gyro_x = isset($_GET['gx']) ? intval($_GET['gx']) : null;
$gyro_y = isset($_GET['gy']) ? intval($_GET['gy']) : null;
$gyro_z = isset($_GET['gz']) ? intval($_GET['gz']) : null;

// Validate data
if ($distance === null) {
    echo json_encode([
        'status' => 'error',
        'message' => 'No distance data provided'
    ]);
    exit();
}

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    echo json_encode([
        'status' => 'error',
        'message' => 'Database connection failed'
    ]);
    exit();
}

// Prepare and execute SQL query
$sql = "INSERT INTO sensor_data (distance, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z) VALUES (?, ?, ?, ?, ?, ?, ?)";
$stmt = $conn->prepare($sql);
$stmt->bind_param("diiiiii", $distance, $acc_x, $acc_y, $acc_z, $gyro_x, $gyro_y, $gyro_z);

if ($stmt->execute()) {
    echo json_encode([
        'status' => 'success',
        'message' => 'Data saved successfully'
    ]);
} else {
    echo json_encode([
        'status' => 'error',
        'message' => 'Failed to save data'
    ]);
}

$stmt->close();
$conn->close();
?>