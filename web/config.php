<?php
// WiFi Network Configuration
define('ESP32_IP', '192.168.137.163'); // ESP32's static IP address - update this to match your ESP32's IP

// Database Configuration
define('DB_SERVER', 'localhost');
define('DB_USERNAME', 'root');
define('DB_PASSWORD', '');
define('DB_NAME', 'robot_db');

// Error reporting
ini_set('display_errors', 1);
error_reporting(E_ALL);
?>