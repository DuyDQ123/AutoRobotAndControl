# Robot Control Web Interface

This web interface allows you to control and monitor the robot using ESP32-CAM and Arduino UNO.

## Setup Instructions

1. **Database Setup**:
   - Open XAMPP Control Panel
   - Start Apache and MySQL services
   - Open phpMyAdmin (http://localhost/phpmyadmin)
   - Import `setup_database.sql` to create the database and tables

2. **Web Files Setup**:
   - Copy all files from this `web` folder to `C:/xampp/htdocs/robot/`
   - Edit ESP32-CAM code to update `webserver` variable with your PC's IP address

3. **Hardware Setup**:
   - Upload `esp32cam.ino` to ESP32-CAM
   - Upload `arduinouno.ino` to Arduino UNO
   - Ensure both devices are powered and connected

4. **Network Setup**:
   - Connect to the WiFi network specified in ESP32-CAM code (SSID: "duy", password: "11111111")
   - Note the ESP32-CAM's IP address printed in Serial Monitor

## Using the Interface

1. Open web interface: `http://localhost/robot/`
2. The interface shows:
   - Live camera feed from ESP32-CAM
   - Robot control buttons (Forward, Backward, Left, Right, Stop)
   - Sensor data display (Distance, Temperature, Humidity)

## File Structure

- `index.html` - Main web interface
- `control.php` - Handles robot control commands
- `get_sensor_data.php` - Retrieves latest sensor data from database
- `get_camera_feed.php` - Handles camera stream from ESP32-CAM
- `save_sensor_data.php` - Saves sensor data to database
- `setup_database.sql` - Database creation script

## Troubleshooting

1. If camera feed is not visible:
   - Check ESP32-CAM connection
   - Verify camera initialization in ESP32-CAM code
   - Check if ESP32-CAM IP address is correct

2. If sensor data is not updating:
   - Check Arduino UNO connection
   - Verify serial communication between ESP32-CAM and Arduino
   - Check database connection settings

3. If controls are not responding:
   - Check WiFi connection
   - Verify ESP32-CAM IP address
   - Check serial communication between boards