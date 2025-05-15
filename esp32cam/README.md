# ESP32-CAM Code Structure

## Cấu trúc thư mục
```
esp32cam/
├── esp32cam.ino      # File chính
├── config.h          # Cấu hình WiFi và server
├── camera_pins.h     # Định nghĩa chân kết nối camera
├── camera_config.h   # Cấu hình và khởi tạo camera
├── uart_config.h     # Cấu hình giao tiếp UART với Arduino
└── server_config.h   # Cấu hình và xử lý web server
```

## Giao tiếp UART với Arduino
- ESP32-CAM sử dụng UART hardware trên chân:
  - TX: GPIO15 (nối với RX6 của Arduino)
  - RX: GPIO14 (nối với TX5 của Arduino)
- Tốc độ baud: 9600

## Các Web Server
1. Video Server (Port 80):
- Xử lý stream hình ảnh từ camera
- Route `/capture`: Chụp và gửi ảnh JPEG

2. Sensor Server (Port 81):
- Nhận và xử lý dữ liệu cảm biến
- Format dữ liệu: `D:distance,A:accX|accY|accZ,G:gyroX|gyroY|gyroZ`

3. Control Server (Port 82):
- Nhận lệnh điều khiển robot
- Route `/control?command=<COMMAND>`
- Các lệnh:
  - FORWARD (F)
  - BACKWARD (B)
  - LEFT (L)
  - RIGHT (R)
  - STOP (S)
  - CENTER (C)
  - AUTO (A)
  - MANUAL (M)

## Luồng hoạt động

### Khởi tạo (setup)
1. Khởi tạo Serial debug (115200 baud)
2. Thiết lập UART với Arduino (9600 baud)
3. Kết nối WiFi
4. Khởi tạo camera với cấu hình tối ưu
5. Khởi tạo các web server

### Vòng lặp chính (loop)
1. Đọc dữ liệu từ Arduino:
   - Nhận dữ liệu cảm biến
   - Parse và gửi lên web server

2. Kiểm tra kết nối WiFi:
   - Tự động kết nối lại nếu mất kết nối

3. Xử lý các request:
   - Xử lý request chụp ảnh
   - Xử lý request điều khiển
   - Xử lý dữ liệu cảm biến

## Chú ý
- Code được tổ chức theo modules để dễ bảo trì và mở rộng
- Sử dụng hardware UART để giao tiếp với Arduino đảm bảo độ ổn định
- Log đầy đủ thông tin debug qua Serial để dễ theo dõi
- Có cơ chế tự động kết nối lại WiFi khi mất kết nối
