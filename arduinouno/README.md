# Arduino UNO Code Structure

## Cấu trúc thư mục
```
arduinouno/
├── arduinouno.ino     # File chính
├── uart_config.h/.cpp # Giao tiếp Software Serial với ESP32
├── servo_control.h/.cpp # Điều khiển servo
├── sensors.h/.cpp     # Xử lý cảm biến
└── auto_navigation.h/.cpp # Điều hướng tự động
```

## Kết nối phần cứng

### 1. Giao tiếp với ESP32-CAM
- Sử dụng Software Serial
- RX: D6 (UNO) <- GPIO15 (ESP32)
- TX: D5 (UNO) -> GPIO14 (ESP32)
- Tốc độ baud: 9600

### 2. Servo Motors
- Servo trái: Pin 11
- Servo phải: Pin 10

### 3. Cảm biến siêu âm HC-SR04
- TRIG: Pin 8
- ECHO: Pin 9
- Khoảng cách đo tối đa: 200cm

### 4. Cảm biến IMU MPU6050
- Kết nối qua I2C
- Đo gia tốc và góc quay

## Giao thức truyền nhận

### 1. Nhận lệnh từ ESP32
- Lệnh 1 ký tự:
  - F: Forward (Tiến)
  - B: Backward (Lùi)
  - L: Left (Rẽ trái)
  - R: Right (Rẽ phải)
  - S: Stop (Dừng)
  - A: Auto mode (Tự động)
  - M: Manual mode (Điều khiển)

### 2. Gửi dữ liệu lên ESP32
- Format: `D:distance,A:accX|accY|accZ,G:gyroX|gyroY|gyroZ`
- Chu kỳ gửi: 500ms
- Dữ liệu bao gồm:
  - Khoảng cách (cm)
  - Gia tốc (3 trục)
  - Góc quay (3 trục)

## Cơ chế hoạt động

### 1. Khởi tạo (setup)
1. Khởi tạo Serial debug (115200 baud)
2. Khởi tạo Software Serial với ESP32 (9600 baud)
3. Thiết lập servo
4. Khởi tạo cảm biến MPU6050

### 2. Vòng lặp chính (loop)
1. Kiểm tra lệnh từ ESP32:
   - Đọc qua Software Serial
   - Thực thi lệnh di chuyển
   - Chuyển đổi chế độ Auto/Manual

2. Nếu ở chế độ Auto:
   - Thực hiện điều hướng tự động
   - Tránh vật cản tự động

3. Cập nhật dữ liệu cảm biến:
   - Đọc khoảng cách và IMU
   - Định dạng dữ liệu
   - Gửi qua Software Serial
   - Chu kỳ cập nhật: 500ms

### 3. Debug
- Hardware Serial (115200 baud): In log debug
- Format log: `[DEBUG/INFO/ERROR] message`
- Thông tin debug bao gồm:
  - Trạng thái khởi tạo
  - Lệnh nhận được
  - Dữ liệu gửi đi
  - Lỗi nếu có
