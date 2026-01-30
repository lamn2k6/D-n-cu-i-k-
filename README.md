1. Mục đích dự án (Project Objectives)
&emsp;Bảo vệ phần cứng: Triệt tiêu xung sét bằng hệ thống phân tầng (Varistor & GDT); Ngắt tải khẩn cấp khi quá áp (>250V) qua SSR.

&emsp;Đo lường: Giám sát 06 thông số điện năng (V, I, P, kWh, Hz, PF) với sai số < 1%.

&emsp;Duy trì kết nối: Tính năng "Last Gasp" sử dụng siêu tụ điện để gửi cảnh báo mất nguồn AC về máy chủ trước khi thiết bị tắt hoàn toàn.

&emsp;Mạng lưới: Quản lý tập trung qua cấu trúc Master-Slave (ESP-NOW) và Bridge sang MQTT.

2. Quy trình Triển khai (Build & Run)
&emsp;Phần cứng (Hardware)
&emsp;Kết nối linh kiện theo sơ đồ tại /docs/schematics.

&emsp;Yêu cầu kỹ thuật: * Sử dụng dây dẫn đơn lõi tiết diện lớn cho mạch động lực (40A).

&emsp;Bắt buộc sử dụng Heatsink và keo tản nhiệt cho SSR-40 DA.

&emsp;Đấu nối an toàn qua cầu đấu Domino 60A.

&emsp;Phần mềm (Software)
&emsp;Firmware: * Nạp mã nguồn cho Master và Slave (sử dụng Arduino IDE/PlatformIO).

&emsp;Thư viện yêu cầu: PZEM004Tv30, PubSubClient, ArduinoJson.

&emsp;Server & Dashboard:

&emsp;Cấu hình MQTT Broker (HiveMQ/Mosquitto).

&emsp;Import Flow vào Node-RED từ tệp /server/nodered_flows.json.

&emsp;Kết nối và đồng bộ dữ liệu với Home Assistant.

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
