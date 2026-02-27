# 🎓 Classroom Management System - Complete IoT Solution

**Status: ✅ COMPLETE AND READY TO USE**

A complete, production-ready IoT classroom management system with ESP32 firmware, Node.js backend, web dashboard, and RFID attendance tracking.

---

## 🔗 About

An integrated IoT solution for smart classroom management featuring real-time environmental monitoring (temperature, humidity, CO levels, noise), automatic RFID-based attendance tracking, and a responsive web dashboard with live data visualization. Perfect for educational institutions and smart building systems.

**Key Highlights:**
- 🔧 **Hardware**: ESP32 microcontroller with 5 integrated sensors
- 📡 **Backend**: Node.js Express API with SQLite database
- 🎨 **Frontend**: Modern responsive web dashboard with Chart.js
- 🏷️ **RFID**: Student card registration and attendance logging
- ⚠️ **Alerts**: Real-time environmental alert system
- 📊 **Analytics**: Historical data tracking and CSV export
- 📚 **Documented**: 7 comprehensive guides included

---

## 📋 Table of Contents
1. [System Overview](#system-overview)
2. [Quick Start (5 Minutes)](#quick-start-5-minutes)
3. [Hardware Setup](#hardware-setup)
4. [Software Installation](#software-installation)
5. [API Documentation](#api-documentation)
6. [Dashboard Features](#dashboard-features)
7. [Troubleshooting](#troubleshooting)
8. [Project Structure](#project-structure)

---

## 🚀 Quick Links
- **[Quick Start Guide](QUICKSTART.md)** - Get running in 5 minutes
- **[API Documentation](API.md)** - Complete API endpoint reference
- **[Hardware Pins](PIN_REFERENCE.md)** - ESP32 pin configuration
- **[Testing Guide](TESTING_GUIDE.md)** - Full testing procedures
- **[Project Structure](PROJECT_STRUCTURE.md)** - File organization

---

## ⚡ Getting Started

### Prerequisites
- ESP32 development board with USB cable
- Arduino IDE (latest version)
- Node.js (LTS version)
- WiFi network (2.4GHz)
- All required sensors and components

### Installation (3 Steps)

**1. Upload ESP32 Firmware**
```bash
# Edit a.ino lines 11-13 with your WiFi credentials:
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
const char* apiUrl = "http://YOUR_PC_IP:5000/api";

# Then upload to ESP32 in Arduino IDE
```

**2. Start Backend Server**
```bash
npm install
npm start
# Server runs on http://localhost:5000
```

**3. Open Dashboard**
```
Browser → http://localhost:5000
```

👉 **[Full guide →](QUICKSTART.md)**

---

## System Overview

### 📦 What You Have

A **complete, production-ready IoT classroom management system** with:
- ✅ ESP32 firmware for 5 sensors + RFID reader
- ✅ Node.js backend API server (Express.js)
- ✅ Interactive web dashboard with real-time charts
- ✅ RFID attendance tracking system
- ✅ Alert & monitoring system
- ✅ SQLite database for data persistence
- ✅ Complete documentation

### 🏗️ System Architecture

```
CLASSROOM ENVIRONMENT
        ↓
    [ESP32 MICROCONTROLLER]
    ├─ Sound Sensor (GPIO35)
    ├─ DHT22 Temp/Humidity (GPIO25)
    ├─ MQ7 CO Sensor (GPIO32)
    ├─ RFID RC522 (GPIO5,27,19,23,21)
    └─ Status LED (GPIO2)
        ↓ (WiFi HTTP)
    [NODE.JS SERVER:5000]
    ├─ routes/sensors.js
    ├─ routes/rfid.js
    ├─ routes/dashboard.js
    ├─ database.js (SQLite)
    └─ public/index.html
        ↓
    [WEB DASHBOARD]
    ├─ Real-time Charts
    ├─ RFID Management
    ├─ Attendance Tracking
    └─ Alert System
```

---

## Quick Start (5 Minutes)

### Prerequisites
- ESP32 microcontroller with all sensors connected
- Node.js installed on your PC
- USB cable for ESP32 programming
- WiFi network (2.4GHz)
- PC with `npm start` capability

### Step 1: Upload ESP32 Firmware (2 minutes)

**Arduino IDE:**
```
1. Open a.ino file
2. Edit lines 11-13 with your WiFi:
   const char* ssid = "YOUR_WIFI";
   const char* password = "YOUR_PASSWORD";
   const char* apiUrl = "http://192.168.1.X:5000/api";  // Your PC IP

3. Select Board: ESP32 Dev Module
4. Click Upload (Ctrl+U)
5. Wait for "Done uploading"
```

**Serial Monitor Output (verify):**
```
Connecting to WiFi: YOUR_NETWORK
WiFi Connected!
IP Address: 192.168.1.X
Synchronizing time with NTP...
✓ Time synchronized successfully!
System ready!
```

### Step 2: Start Backend Server (1 minute)

```bash
# Navigate to project directory
cd [your-project-directory]
npm install
npm start
```

**Expected Output:**
```
Classroom Management API Server running on port 5000
Database initialized
Dashboard available at http://localhost:5000
```

### Step 3: Open Web Dashboard (0.5 minutes)

```
http://localhost:5000
```

You should see the dashboard with real-time data! 🎉

### Step 4: Register RFID Cards (optional)

1. Go to **"✏️ Register RFID Card"** tab
2. Enter Student Name
3. Click **"🎯 Start RFID Scan"**
4. Scan RFID card - UID auto-populates
5. Click **Register Card**

---

## Hardware Setup

### 🔌 Wiring Diagram - ESP32 Pin Configuration

```
ESP32 DEVKIT PINOUT (Top View):

LEFT SIDE                    RIGHT SIDE
═══════════════════════════════════════════
GND ┐                          ┌ 3V3
    │  ┌─────────────────────┐ │
    │  │   ESP32 DevKit      │ │ GPIO35 (SOUND)
    │  │                     │ │ EN
    │  │   Main Pins Used:   │ │ GPIO36
    │  │  · GPIO27 (RST)     │ │ GPIO39
    │  │  · GPIO5 (CS)       │ │ GPIO34
    │  │  · GPIO19 (MOSI)    │ │ GPIO35 (SOUND)
    │  │  · GPIO23 (SCK)     │ │ GND
    │  │  · GPIO21 (MISO)    │ │ GPIO32 (MQ7)
    │  │  · GPIO25 (DHT22)   │ │ GPIO33
    │  │  · GPIO2 (LED)      │ │ GPIO25 (DHT)
    │  │  · GPIO13 (WiFi)    │ │ GPIO26
    │  │                     │ │ GPIO27 (RST)
    │  │                     │ │ GPIO14
    │  │                     │ │ GPIO12
    │  │                     │ │ GND
    │  │                     │ │ GPIO13 (WiFi)
    │  └─────────────────────┘ │
    │                          │
GND ─                          ─ 5V
```

### 📌 ESP32 Pinout Reference

| Sensor | GPIO | Physical Pin | Hardware Pin |
|--------|------|------------|-----------------|
| **Sound Sensor** | 35 | Right-3 | VP (Analog) |
| **DHT22** | 25 | Left-13 | D25 (Digital) |
| **MQ7 CO** | 32 | Right-8 | SVP (Analog) |
| **RFID CS/SDA** | 5 | Left-15 | D5 (SPI) |
| **RFID RST** | 27 | Left-11 | D27 (Output) |
| **RFID MOSI** | 19 | SPI Bus | D19 (SPI) |
| **RFID SCK** | 23 | SPI Bus | D23 (SPI) |
| **RFID MISO** | 21 | SPI Bus | D21 (SPI) |
| **WiFi LED** | 13 | Left-8 | D13 (Output) |
| **Built-in LED** | 2 | Left-7 | D2 (Output) |

### 🔗 Component Connections

**DHT22 Sensor (3-pin):**
```
DHT22 VDD (Pin 1)  → ESP32 5V (with 100nF capacitor)
DHT22 DATA (Pin 2) → ESP32 GPIO25
DHT22 GND (Pin 3)  → ESP32 GND
```

**RFID RC522 Module (8-pin):**
```
RC522 3.3V   → ESP32 3.3V (+ 100nF decoupling cap)
RC522 RST    → ESP32 GPIO27
RC522 GND    → ESP32 GND
RC522 MISO   → ESP32 GPIO21 (MISO)
RC522 MOSI   → ESP32 GPIO19 (MOSI)
RC522 SCK    → ESP32 GPIO23 (SCK)
RC522 SDA    → ESP32 GPIO5 (CS - Chip Select)
RC522 IRQ    → (Not used - leave disconnected)
```

**MQ7 CO Sensor:**
```
MQ7 VCC  → ESP32 5V
MQ7 A0   → ESP32 GPIO32 (ADC)
MQ7 GND  → ESP32 GND
```

**Sound Sensor:**
```
Sound VCC → ESP32 5V
Sound A0  → ESP32 GPIO35 (ADC/VP)
Sound GND → ESP32 GND
```

---

## Software Installation

### Arduino IDE Setup

#### 1. Install ESP32 Board Support
- **File → Preferences**
- Add to "Additional Board Manager URLs":
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- **Tools → Board → Boards Manager**
- Search "esp32" → Install

#### 2. Install Required Libraries
Go to **Sketch → Include Library → Manage Libraries:**

- **ArduinoJson** (v6.x.x) - JSON serialization
- **MFRC522** (by GithubCommunity) - RFID reader
- **DHT sensor library** (by Adafruit) - Temperature/Humidity

#### 3. Board Configuration
- **Tools → Board** → ESP32 Dev Module
- **Tools → Port** → COM port (where ESP32 is connected)
- **Tools → Upload Speed** → 115200

### Node.js Backend Setup

#### 1. Install Dependencies
```bash
# Navigate to your project directory
cd [your-project-directory]
npm install
```

#### 2. Start Server
```bash
npm start
```

Server runs on `http://localhost:5000`

#### 3. Optional: Run on Different Port
```bash
set PORT=8000
npm start
```

---

## API Documentation

### Base URL
```
http://localhost:5000/api
```

### 📡 Sensor Endpoints

#### POST `/sensor-data`
**ESP32 sends sensor readings**

```json
REQUEST:
{
  "deviceId": "CLASSROOM_01",
  "soundLevel": 45.2,
  "temperature": 24.5,
  "humidity": 55.3,
  "coLevel": 2.1,
  "rfidCard": "1A2B3C4D",
  "timestamp": "2026-02-27 14:30:45",
  "rssi": -67
}

RESPONSE (201):
{
  "success": true,
  "message": "Sensor data recorded",
  "data": {
    "deviceId": "CLASSROOM_01",
    "timestamp": "2026-02-27 14:30:45"
  }
}
```

#### GET `/sensor-data/latest`
**Get most recent reading**

```json
RESPONSE:
{
  "id": 1234,
  "deviceId": "CLASSROOM_01",
  "soundLevel": 45.2,
  "temperature": 24.5,
  "humidity": 55.3,
  "coLevel": 2.1,
  "rssi": -67,
  "timestamp": "2026-02-27 14:30:45"
}
```

#### GET `/sensor-data/history?limit=50`
**Get historical sensor data**

```json
RESPONSE:
[
  { ...full sensor record... },
  { ...full sensor record... }
]
```

#### POST `/sensor-data/generate-test-data`
**Generate 24 hours of test data (for demo)**

```json
RESPONSE:
{
  "success": true,
  "message": "Generated 24 test data records",
  "records": 24
}
```

### 🏷️ RFID Endpoints

#### POST `/rfid/register`
**Register a new RFID card for a student**

```json
REQUEST:
{
  "rfidUid": "1A2B3C4D",
  "studentName": "John Doe"
}

RESPONSE (201):
{
  "success": true,
  "message": "RFID card registered",
  "cardId": "550e8400-e29b-41d4-a716-446655440000",
  "rfidUid": "1A2B3C4D",
  "studentName": "John Doe"
}
```

#### POST `/rfid/scan`
**Store real-time RFID scan (called by ESP32)**

```json
REQUEST:
{
  "rfidUid": "1A2B3C4D"
}

RESPONSE:
{
  "success": true,
  "message": "RFID scan received",
  "rfidUid": "1A2B3C4D",
  "timestamp": "2026-02-27 14:30:45"
}
```

#### GET `/rfid/last-scan`
**Get last scanned RFID UID (for dashboard registration)**

```json
RESPONSE:
{
  "uid": "1A2B3C4D",
  "timestamp": "2026-02-27 14:30:45"
}
```

#### POST `/rfid/log`
**Log RFID card access (entry/exit)**

```json
REQUEST:
{
  "rfidUid": "1A2B3C4D",
  "action": "ENTRY"
}

RESPONSE:
{
  "success": true,
  "message": "Access logged",
  "cardId": "550e8400-e29b-41d4-a716-446655440000",
  "studentName": "John Doe",
  "action": "ENTRY",
  "timestamp": "2026-02-27 14:30:45"
}
```

#### GET `/rfid/cards`
**Get all registered RFID cards**

```json
RESPONSE:
[
  {
    "cardId": "550e8400-e29b-41d4-a716-446655440000",
    "studentName": "John Doe",
    "rfidUid": "1A2B3C4D",
    "createdAt": "2026-02-24 10:00:00"
  }
]
```

#### GET `/rfid/logs?limit=50`
**Get RFID access logs**

```json
RESPONSE:
[
  {
    "id": 1,
    "timestamp": "2026-02-27 14:30:45",
    "action": "ENTRY",
    "studentName": "John Doe",
    "rfidUid": "1A2B3C4D"
  }
]
```

#### GET `/rfid/attendance`
**Get attendance for today**

```json
RESPONSE:
[
  {
    "studentName": "John Doe",
    "rfidUid": "1A2B3C4D",
    "entries": 2,
    "firstEntry": "2026-02-27 08:00:00",
    "lastEntry": "2026-02-27 14:30:00"
  }
]
```

### 📊 Dashboard Endpoints

#### GET `/dashboard/summary`
**Get dashboard summary with latest readings**

```json
RESPONSE:
{
  "latestSensor": { ...sensor data... },
  "attendance": 25,
  "activeAlerts": 2,
  "recentAlerts": [...],
  "stats": {
    "avgSound": 42.5,
    "avgTemp": 24.2,
    "avgHumidity": 54.8,
    "avgCO": 2.3
  }
}
```

#### GET `/dashboard/chart-data?hours=24`
**Get hourly aggregated data for charts**

```json
RESPONSE:
{
  "tempHumidData": [
    {
      "hour": "2026-02-27 08:00:00",
      "avgTemp": 22.5,
      "avgHumidity": 50.2,
      "maxTemp": 23.1,
      "minTemp": 21.8
    }
  ],
  "pollutantData": [
    {
      "hour": "2026-02-27 08:00:00",
      "avgSound": 45.2,
      "avgCO": 2.1
    }
  ],
  "period": "24 hours"
}
```

---

## Dashboard Features

### 📊 Real-Time Monitoring

**Live Sensor Cards:**
- 🔊 Sound Level (0-100%)
- 🌡️ Temperature (-10 to 50°C)
- 💧 Humidity (0-100%)
- ⚠️ CO Level (0-500+ PPM)
- 📊 Current Alerts

**24-Hour Trend Charts:**
- Temperature & Humidity trends
- Sound Level & CO Concentration trends
- Real-time updates every 3 seconds

### 🎫 RFID Management

**Tab 1: Register RFID Card**
- Enter student name
- Click "🎯 Start RFID Scan"
- Scan RFID card
- UID auto-populates
- Click "Register Card"

**Tab 2: RFID Logs**
- View all card scans
- Student name and UID
- Action (ENTRY/EXIT)
- Timestamp

**Tab 3: Attendance**
- Daily attendance records
- First & last entry times
- Number of entries

### ⚠️ Alert System

**Alert Types:**
- Temperature warnings (too hot/cold)
- Excessive noise detection
- High CO levels
- Humidity out of range

**Alert Features:**
- Color-coded by severity
- Timestamps for each alert
- Resolve button to dismiss
- Auto-refresh every 10 seconds

### 🧪 Testing Features

**Generate Test Data Button:**
- Adds 24 hours of sample data
- Instantly populates charts
- Useful for demo & testing
- Real data mixes with test data

---

## Troubleshooting

### ESP32 Issues

#### WiFi Won't Connect
```
Problem: Serial Monitor shows "WiFi Failed!"
Solution:
1. Check WiFi SSID and password in a.ino (lines 11-13)
2. Ensure 2.4GHz WiFi (not 5GHz)
3. Try restarting ESP32 (press RST button)
4. Check router is broadcasting SSID
```

#### No Time Sync
```
Problem: "⚠ Warning: Time sync in progress"
Solution:
1. This is normal - system continues anyway
2. NTP needs ~60 seconds on first boot
3. Time will eventually sync
4. Timestamps will be incorrect until synced
```

#### DHT22 Not Reading
```
Problem: Serial shows "Failed to read DHT22"
Solution:
1. Check GPIO25 connection is secure
2. Verify 100nF capacitor between VCC and GND
3. Use shorter wires (< 1 meter)
4. Ensure DHT22 is powered with 5V
5. Try moving away from sources of electromagnetic noise
```

### Server Issues

#### "Port 5000 already in use"
```bash
# Option 1: Kill process on port 5000
netstat -ano | findstr :5000
taskkill /PID <PID> /F

# Option 2: Use different port
set PORT=8000
npm start
```

#### "ENOENT: no such file or directory, open 'database.js'"
```bash
# Make sure you're in correct directory:
cd [your-project-directory]
npm start
```

#### Database locked error
```bash
# Close dashboard and restart server
npm start
```

### Dashboard Issues

#### Charts Empty/Not Updating
```
1. Click "🧪 Test Data" button to generate sample data
2. Charts should populate in 3 seconds
3. If still empty, check browser console (F12)
4. Verify ESP32 is connected and sending data
```

#### "Can't connect to server"
```
1. Is Node.js running? Check terminal
2. Is ESP32 on same WiFi network?
3. Can you ping PC IP from ESP32?
4. Check firewall not blocking port 5000
```

#### RFID Registration Not Working
```
1. Click "🎯 Start RFID Scan" button
2. Button should turn orange "⏳ Listening..."
3. Scan RFID card near reader
4. UID field should auto-populate
5. If it doesn't:
   - Check RFID reader is powered (LED should glow)
   - Check GPIO5, 27 connections
   - Try scanning slower/closer to reader
```

---

## Project Structure

```
[project-directory]/
│
├── 📄 a.ino                          # Main ESP32 firmware (upload this!)
├── 📄 main/main.ino                  # Alternative ESP32 firmware location
├── 📄 config.h                       # Hardware configuration
│
├── 📄 server.js                      # Node.js Express server
├── 📄 database.js                    # SQLite database handler
├── 📄 package.json                   # Dependencies & scripts
│
├── 🗂️  routes/
│   ├── 📄 sensors.js                 # Sensor & test data endpoints
│   ├── 📄 rfid.js                    # RFID management endpoints
│   └── 📄 dashboard.js               # Dashboard & analytics endpoints
│
├── 🗂️  public/
│   └── 📄 index.html                 # Web dashboard UI
│
├── 🗂️  data/
│   └── 📄 classroom.db               # SQLite database (auto-created)
│
├── 📘 README.md                      # THIS FILE - Complete documentation
├── .env.example                      # Environment variables template
└── .gitignore                        # Git ignore rules
```

### Key Files Overview

| File | Purpose | Edit? |
|------|---------|-------|
| `a.ino` | Main ESP32 code | YES - WiFi credentials |
| `server.js` | Backend API | NO |
| `database.js` | Database handler | NO |
| `config.h` | Pin configuration | YES - if using different pins |
| `public/index.html` | Dashboard UI | NO |
| `routes/*.js` | API endpoints | NO |

---

## System Specifications

### Sensors
- **Sound**: 0-100% (ADC mapping)
- **Temperature**: -10 to 50°C (DHT22)
- **Humidity**: 0-100% (DHT22)
- **CO Level**: 0-500+ PPM (MQ7)
- **Signal Strength**: RSSI value (WiFi)

### Storage
- **Database**: SQLite3
- **Retention**: Unlimited (limited by disk space)
- **Recovery**: Database persists after power loss

### Networking
- **Protocol**: HTTP/JSON over WiFi
- **Port**: 5000 (configurable)
- **Baud Rate**: 115200 (ESP32 serial)
- **Timezone**: Sri Lanka (UTC+5:30)

### Alert Thresholds (Configurable)
- **Temperature**: 18-28°C
- **Humidity**: 30-70%
- **Sound**: 80%
- **CO**: 10 PPM

---

## Support & Tips

### Performance Tips
1. Keep ESP32 and Server on same network
2. Minimize distance from WiFi router
3. Avoid metal obstacles between devices
4. Use 2.4GHz WiFi (better range than 5GHz)

### Best Practices
1. Regularly check RFID reader battery (if wireless)
2. Clean sensor lenses monthly
3. Keep sensors away from direct sunlight
4. Backup database file (`data/classroom.db`) regularly
5. Monitor system logs for errors

### Development Tips
1. Use Serial Monitor to verify sensor readings
2. Generate test data for demo purposes
3. Check browser Console (F12) for JavaScript errors
4. Monitor Node.js console for API errors
5. Use API endpoints directly via cURL for testing

---

## Files Included (Complete List)

✅ **Firmware**
- `a.ino` - Main ESP32 firmware (RECOMMENDED)
- `main/main.ino` - Alternative firmware location

✅ **Backend**
- `server.js` - Express API server
- `database.js` - SQLite handler
- `package.json` - Dependencies
- `routes/sensors.js` - Sensor endpoints (5+ endpoints)
- `routes/rfid.js` - RFID endpoints (8+ endpoints)
- `routes/dashboard.js` - Dashboard endpoints (6+ endpoints)

✅ **Frontend**
- `public/index.html` - Web dashboard

✅ **Testing & Diagnostics**
- `sensor_test.ino` - Hardware diagnostic tool
- `config.h` - Hardware pin configuration

✅ **Configuration**
- `.env.example` - Environment variables template
- `.gitignore` - Git ignore rules

✅ **Documentation (7 Guides)**
- `README.md` - This comprehensive guide (3000+ words)
- `QUICKSTART.md` - 5-minute quick start (200 words)
- `API.md` - API endpoint reference (1000+ words)
- `PIN_REFERENCE.md` - Pin configuration guide
- `PROJECT_STRUCTURE.md` - File organization (300+ words)
- `TESTING_GUIDE.md` - Complete testing procedures (800+ words)
- `SYSTEM_COMPLETE.md` - System overview & features

---

## Next Steps

1. **Upload ESP32**: Use Arduino IDE to upload `a.ino`
2. **Start Server**: Run `npm start`
3. **Open Dashboard**: Visit `http://localhost:5000`
4. **Register Students**: Add RFID cards to track attendance
5. **Monitor System**: Watch real-time data and alerts

---

## Version Information

- **System**: Classroom Management System v2.0
- **ESP32 Firmware**: Complete with NTP, RFID, 5 Sensors
- **Backend**: Node.js Express with SQLite
- **Dashboard**: Modern responsive web UI
- **Status**: ✅ Production Ready

---

## License & Credits

This is a complete educational IoT system designed for classroom management and environmental monitoring.

**Built with:**
- Arduino IDE - ESP32 programming
- Node.js/Express - Backend API
- SQLite3 - Data storage
- Chart.js - Real-time visualization
- Responsive HTML/CSS/JavaScript - Frontend

---

## 📦 Technology Stack

| Component | Technology | Purpose |
|-----------|-----------|---------|
| **Microcontroller** | ESP32 Dev Module | Sensor data collection & WiFi |
| **Sensors** | DHT22, MQ7, Sound Module | Environmental monitoring |
| **RFID** | RC522 Module | Student identification |
| **Backend** | Node.js + Express | REST API server |
| **Database** | SQLite3 | Data persistence |
| **Frontend** | HTML5, CSS3, JavaScript | Web dashboard |
| **Charts** | Chart.js | Data visualization |
| **Communication** | HTTP/JSON, WiFi | Data transmission |

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

**To contribute:**
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 🆘 Support

- 📖 Read the comprehensive documentation in the guides
- 🔍 Check [TESTING_GUIDE.md](TESTING_GUIDE.md) for troubleshooting
- 💡 See [API.md](API.md) for endpoint details
- 📌 Refer to [PIN_REFERENCE.md](PIN_REFERENCE.md) for hardware pins

---

**Last Updated:** February 28, 2026  
**System Status:** ✅ COMPLETE AND OPERATIONAL
#   c l a s s r o o m - m a n a g e m e n t - s y s t e m 
 
 #   c l a s s r o o m - m a n a g e m e n t - s y s t e m 
 
 #   c l a s s r o o m - m a n a g e m e n t - s y s t e m 
 
 #   c l a s s r o o m - m a n a g e m e n t - s y s t e m  
 