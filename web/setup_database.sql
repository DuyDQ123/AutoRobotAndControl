-- Create the database if it doesn't exist
CREATE DATABASE IF NOT EXISTS robot_db;

-- Use the database
USE robot_db;

-- Create sensor data table
CREATE TABLE IF NOT EXISTS sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    distance FLOAT,
    acc_x INT,
    acc_y INT,
    acc_z INT,
    gyro_x INT,
    gyro_y INT,
    gyro_z INT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create an index on timestamp for faster queries
CREATE INDEX idx_timestamp ON sensor_data(timestamp);