# Project Structure - Classroom Management System

```
[project-directory]/
│
├── 📄 a.ino                          # Main ESP32 firmware - UPLOAD THIS FILE!
├── 📄 main/main.ino                 # Alternative firmware location (same code as a.ino)
├── 📄 sensor_test.ino               # Diagnostic sketch for sensor testing
│
├── 📄 config.h                       # Hardware configuration (optional)
├── 📄 .env.example                   # Environment variables example
├── 📄 .gitignore                     # Git ignore rules
│
├── 📄 server.js                      # Node.js API server entry point
├── 📄 database.js                    # SQLite database handler
├── 📄 package.json                   # Node.js dependencies
│
├── 🗂️  routes/                        # Backend API route handlers
│   ├── 📄 sensors.js                 # Sensor data endpoints
│   ├── 📄 rfid.js                    # RFID management endpoints
│   └── 📄 dashboard.js               # Dashboard & analytics endpoints
│
├── 🗂️  public/                        # Frontend files (served by Express)
│   └── 📄 index.html                 # Web dashboard UI
│
├── 🗂️  data/                          # Database storage (auto-created)
│   └── 📄 classroom.db               # SQLite database file
│
├── 🗂️  logs/                          # Application logs (optional)
│
├── 📘 README.md                      # Complete documentation & setup guide
├── 📗 QUICKSTART.md                  # 5-minute quick setup guide
├── 📙 API.md                         # API endpoint documentation
└── 📄 PROJECT_STRUCTURE.md           # This file

```

---

## 📥 Installation & Usage Files

| File | Purpose | Action |
|------|---------|--------|
| `a.ino` | Main firmware code (RECOMMENDED) | Upload to ESP32 via Arduino IDE |
| `main/main.ino` | Alternative firmware location | Use if a.ino not found |
| `sensor_test.ino` | Sensor diagnostic tool | Use to test individual sensors |
| `config.h` | Hardware configuration | Edit only for custom pins |

---

## 💻 Backend Server Files

| File | Purpose |
|------|---------|
| `server.js` | Main Express server - run with `npm start` |
| `database.js` | SQLite database interface |
| `package.json` | Dependencies and scripts |
| `routes/sensors.js` | Sensor data API endpoints |
| `routes/rfid.js` | RFID management endpoints |
| `routes/dashboard.js` | Dashboard & analytics endpoints |

---

## 🌐 Frontend Files

| File | Purpose |
|------|---------|
| `public/index.html` | Web dashboard UI (responsive design) |
| | Access at: `http://localhost:5000` |

---

## 📚 Documentation Files

| File | Read When |
|------|-----------|
| `README.md` | You want complete documentation |
| `QUICKSTART.md` | You want to get running in 5 minutes |
| `API.md` | You need API endpoint details |

---

## 🗂️ Auto-Generated Directories

| Directory | Purpose |
|-----------|---------|
| `data/` | SQLite database storage |
| `logs/` | Application logs (if logging enabled) |
| `node_modules/` | NPM packages (created by npm install) |

---

## 🔄 Typical Workflow

### First Time Setup:
1. **Edit configuration:**
   - `a.ino` - WiFi credentials (lines 11-13)
   - `config.h` - Adjust if using different pins

2. **Upload ESP32:**
   - Open `a.ino` in Arduino IDE
   - Click Upload

3. **Start server:**
   - Open terminal/command prompt
   - Navigate: `cd [your-project-directory]`
   - Run: `npm install` (first time only)

3. **Start server:**
   - Open terminal
   - `npm install`
   - `npm start`

4. **Access dashboard:**
   - Open `http://localhost:5000`

### Daily Usage:
- Monitor dashboard in real-time
- Check alerts and attendance
- Export data if needed

### Troubleshooting:
- Use `sensor_test.ino` to diagnose hardware issues
- Check ESP32 Serial Monitor output
- Review API.md for endpoint testing

---

## 📊 Data Flow Diagram

```
ESP32 (Hardware)
    ↓
    │ WiFi (JSON)
    ↓
Node.js Server (server.js)
    ↓
    ├─→ Database (classroom.db)
    │   └─→ sensor_data, rfid_cards, rfid_logs, alerts
    │
    └─→ Routes (Express.js)
        ├─→ sensors.js (POST/GET sensor data)
        ├─→ rfid.js (POST/GET RFID data)
        └─→ dashboard.js (Analytics & export)
            ↓
            └─→ Web Dashboard (index.html)
                └─→ Charts, alerts, tables
```

---

## 🔌 Hardware Connection Summary

```
ESP32 Pins Used:
- GPIO35  → Sound Sensor (ADC)
- GPIO25  → DHT22 (Temperature/Humidity)
- GPIO32  → MQ7 (CO Sensor - ADC)
- GPIO5   → RFID SS
- GPIO27  → RFID RST
- GPIO19  → RFID MOSI
- GPIO21  → RFID MISO
- GPIO23  → RFID SCK
- GPIO13  → LED WiFi indicator
- GPIO12  → LED Data indicator
- GPIO26  → Buzzer
```

---

## 🗄️ Database Schema

### Tables:
1. **sensor_data** - All sensor readings (temperature, humidity, CO, sound)
2. **rfid_cards** - Registered student cards
3. **rfid_logs** - Student entry/exit logs
4. **alerts** - System alerts (temperature, CO, etc.)

---

## 🚀 Running the System

### Step 1: Upload Firmware
```bash
# In Arduino IDE
1. Open a.ino
2. Select: Tools > Board > ESP32 Dev Module
3. Click Upload
```

### Step 2: Start Backend
```bash
# In Command Prompt/Terminal
cd [your-project-directory]
npm install       # First time only
npm start         # Start server
```

### Step 3: Access Dashboard
```
Open browser → http://localhost:5000
```

---

## 📈 Features Overview

### ✅ Implemented
- Real-time sensor monitoring (Sound, Temp, Humidity, CO)
- RFID card registration and access logging
- Attendance tracking by date
- Alert system with thresholds
- Interactive web dashboard
- Historical data storage
- Chart visualization (24-hour trends)
- Data export (CSV format)
- API endpoints for all functions

### 🎯 Future Enhancements
- WebSocket for real-time updates
- Email/SMS alerts
- Mobile app (React Native)
- Multi-classroom support
- Machine learning anomaly detection
- User authentication
- Role-based access control
- Database backup automation
- Cloud synchronization

---

## 🔧 Customization Guide

### Change Sensor Pins:
Edit `a.ino` lines 17-31

### Change Alert Thresholds:
Edit `routes/sensors.js` lines 68-76

### Change WiFi Settings:
Edit `a.ino` lines 11-13

### Change API Port:
Edit `server.js` line 21: `const PORT = 8080;`

### Change Serial Baud Rate:
Edit `a.ino` line 44: `Serial.begin(9600);`

---

## 🎓 Educational Use Cases

1. **Attendance Management** - RFID card tracking
2. **Environmental Monitoring** - Temperature, humidity tracking
3. **Classroom Safety** - CO level monitoring
4. **Noise Level Management** - Sound detection alerts
5. **IoT Learning** - Students learn sensor integration
6. **Data Analytics** - Historical trend analysis
7. **Web Development** - Dashboard UI customization

---

## 🆘 Common Issues & Solutions

| Issue | Solution |
|-------|----------|
| ESP32 not uploading | Check USB driver, select right COM port |
| WiFi not connecting | Verify SSID/password, ensure 2.4GHz |
| No data in dashboard | Check backend running, ESP32 connected |
| Sensors reading 0 | Run `sensor_test.ino` to diagnose |
| Port 5000 in use | Change port in server.js or use `--port 5001` |

---

## 📞 Support Resources

- **Quick Setup:** Read `QUICKSTART.md`
- **Full Documentation:** Read `README.md`
- **API Details:** Read `API.md`
- **Wiring Help:** See Arduino IDE diagrams in `README.md`
- **Sensor Testing:** Use `sensor_test.ino`

---

## 📦 Required Libraries Summary

### Arduino IDE:
- DHT Sensor Library (Adafruit)
- MFRC522 (GithubCommunity)
- ArduinoJson (v6+)

### Node.js (npm):
- express
- cors
- sqlite3
- body-parser
- uuid

---

**Last Updated:** February 28, 2026  
**System Version:** 1.0.0
**System Version:** 1.0.0
