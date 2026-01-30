1. Mục đích dự án (Project Objectives)
Bảo vệ phần cứng: Triệt tiêu xung sét bằng hệ thống phân tầng (Varistor & GDT); Ngắt tải khẩn cấp khi quá áp (>250V) qua SSR.

Đo lường: Giám sát 06 thông số điện năng (V, I, P, kWh, Hz, PF) với sai số < 1%.

Duy trì kết nối: Tính năng "Last Gasp" sử dụng siêu tụ điện để gửi cảnh báo mất nguồn AC về máy chủ trước khi thiết bị tắt hoàn toàn.

Mạng lưới: Quản lý tập trung qua cấu trúc Master-Slave (ESP-NOW) và Bridge sang MQTT.

2. Quy trình Triển khai (Build & Run)
Phần cứng (Hardware)
Kết nối linh kiện theo sơ đồ tại /docs/schematics.

Yêu cầu kỹ thuật: * Sử dụng dây dẫn đơn lõi tiết diện lớn cho mạch động lực (40A).

Bắt buộc sử dụng Heatsink và keo tản nhiệt cho SSR-40 DA.

Đấu nối an toàn qua cầu đấu Domino 60A.

Phần mềm (Software)
Firmware: * Nạp mã nguồn cho Master và Slave (sử dụng Arduino IDE/PlatformIO).

Thư viện yêu cầu: PZEM004Tv30, PubSubClient, ArduinoJson.

Server & Dashboard:

Cấu hình MQTT Broker (HiveMQ/Mosquitto).

Import Flow vào Node-RED từ tệp /server/nodered_flows.json.

Kết nối và đồng bộ dữ liệu với Home Assistant.

3. Cấu trúc thư mục (Directory Structure)
```cpp
├── src/                    # Source code firmware (C++)
│   ├── master/             # Gateway điều phối dữ liệu
│   └── slave/              # Logic điều khiển ổ cắm đầu cuối
├── docs/                   # Tài liệu kỹ thuật
│   ├── schematics/         # Sơ đồ nguyên lý (EasyEDA)
│   └── hardware_images/    # Hình ảnh thực tế linh kiện & sản phẩm
├── server/                 # Cấu hình phía Server
│   └── nodered/            # Flow xử lý dữ liệu và Dashboard
└── README.md               # Hướng dẫn dự án
```
