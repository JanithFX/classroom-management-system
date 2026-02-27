# System Architecture & Complete Testing Guide

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     CLASSROOM ENVIRONMENT                        │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ↓
        ┌─────────────────────────────────────┐
        │         ESP32 MICROCONTROLLER        │
        │  (Sensor Data Collection & WiFi)    │
        ├─────────────────────────────────────┤
        │  Sensors:                            │
        │  ├─ Sound Sensor (GPIO35)           │
        │  ├─ DHT22 (GPIO25)                  │
        │  ├─ MQ7 CO (GPIO32)                 │
        │  └─ RFID RC522 (GPIO5,27,19,23,21) │
        │                                      │
        │  Indicators:                         │
        │  ├─ WiFi LED (GPIO13)               │
        │  ├─ Data LED (GPIO12)               │
        │  └─ Buzzer (GPIO26)                 │
        └─────────────────────────────────────┘
                        │
                        │ WiFi
                        │ JSON over HTTP
                        │
                        ↓
        ┌──────────────────────────────────────┐
        │    NODE.JS SERVER (port 5000)         │
        ├──────────────────────────────────────┤
        │  ├─ server.js (Express.js)           │
        │  ├─ database.js (SQLite3 handler)    │
        │  └─ routes/                          │
        │      ├─ sensors.js (POST/GET)        │
        │      ├─ rfid.js (RFID endpoints)     │
        │      └─ dashboard.js (Analytics)     │
        └──────────────────────────────────────┘
                        │
        ┌───────────────┼────────────────┐
        ↓               ↓                ↓
    DATABASE      WEB DASHBOARD     API CLIENTS
   (classroom.db)  (index.html)    (cURL, fetch)
                                        
    Tables:           Charts:
    ├─ sensor_data    ├─ Temperature
    ├─ rfid_cards     ├─ Humidity
    ├─ rfid_logs      ├─ Sound Level
    └─ alerts         └─ CO Level
                      
                      Tables:
                      ├─ RFID Logs
                      └─ Attendance
```

---

## 📡 Data Flow Sequence

```
TIME →

1. Data Collection Phase (every 10 seconds):
   ESP32 reads sensors:
   ├─ Analog: Sound (GPIO35), CO (GPIO32)
   ├─ Digital: Temp/Humidity (GPIO25)
   ├─ Serial: RFID (SPI pins)
   └─ Creates JSON payload

2. WiFi Transmission Phase:
   ESP32 → WiFi → PC on same network

3. Server Reception Phase:
   Node.js API:
   ├─ POST /api/sensor-data (from ESP32)
   ├─ Store in SQLite
   ├─ Check thresholds → Create alerts
   └─ Return 201 Created

4. Dashboard Update Phase:
   Web Browser:
   ├─ GET /api/dashboard/summary
   ├─ GET /api/dashboard/chart-data
   ├─ GET /api/alerts
   ├─ GET /api/rfid/logs
   └─ Update in real-time

5. RFID Event Phase (whenever card detected):
   RFID Reader → RC522 → GPIO5 (SS)
   └─ ESP32 reads UID
   └─ Sends to /api/rfid/log
```

---

## ✅ Pre-Deployment Checklist

### Hardware Verification
- [ ] ESP32 board purchased and USB cable available
- [ ] All 5 sensors obtained:
  - [ ] Sound detection module
  - [ ] DHT22 sensor
  - [ ] MQ7 CO sensor
  - [ ] RFID RC522 module
  - [ ] Status LEDs & buzzer
- [ ] Breadboard and jumper wires
- [ ] Power supply (5V)

### Software Setup
- [ ] Node.js installed (LTS version)
- [ ] Arduino IDE installed
- [ ] All required libraries installed in Arduino IDE:
  - [ ] DHT sensor library
  - [ ] MFRC522
  - [ ] ArduinoJson
- [ ] WiFi network available (2.4GHz)
- [ ] PC IP address noted

### Configuration
- [ ] WiFi SSID and password ready
- [ ] PC IP address configured in a.ino
- [ ] Sensor pins verified against circuit
- [ ] Directory structure created

---

## 🧪 Complete Testing Procedure

### Phase 1: Hardware Testing (Use sensor_test.ino)

**Step 1: Upload Diagnostic Sketch**
```
1. Open sensor_test.ino in Arduino IDE
2. Select ESP32 board and COM port
3. Click Upload
4. Open Serial Monitor (115200 baud)
```

**Step 2: Verify Each Sensor**

Check Serial Monitor output for:
```
✓ Sound Sensor: OK
✓ DHT22 Sensor: OK
✓ MQ7 CO Sensor: OK
✓ RFID RC522: OK
```

### Phase 2: WiFi Connectivity Test

**Step 1: Configure WiFi**
```cpp
// Edit in a.ino lines 11-13
const char* ssid = "YOUR_NETWORK";
const char* password = "YOUR_PASSWORD";
const char* apiUrl = "http://192.168.1.X:5000/api"; // Your PC IP
```

**Step 2: Upload Main Firmware**
```
1. Open a.ino in Arduino IDE
2. Click Upload
3. Wait for "Sketch uses X bytes"
4. Open Serial Monitor
```

**Step 3: Check WiFi Connection**
Serial Monitor should show:
```
Connecting to WiFi: YOUR_NETWORK
.....
WiFi Connected!
IP Address: 192.168.1.X
System ready!
```

### Phase 3: Backend Server Testing

**Step 1: Install Dependencies**
```bash
cd [your-project-directory]
npm install
```

**Step 2: Start Server**
```bash
npm start
```

Expected output:
```
Classroom Management API Server running on port 5000
Database connected
Dashboard available at http://localhost:5000
```

**Step 3: Test API Health**
```bash
# In another terminal
curl http://localhost:5000/health
# Should return: {"status":"OK","timestamp":"..."}
```

### Phase 4: Data Reception Test

**Step 1: Monitor Server Logs**
Keep terminal running with `npm start`

**Step 2: Check ESP32 Data**
ESP32 Serial Monitor should show:
```
--- Sensor Readings ---
Sound Level: 45.2%
Temperature: 24.5°C
Humidity: 55.3%
CO Level: 2.1 PPM
Data sent successfully!
```

**Step 3: Verify in Server Terminal**
Backend should log:
```
Data received from CLASSROOM_01 at 2026-02-24 10:30:45
```

### Phase 5: Dashboard Testing

**Step 1: Open Dashboard**
```
Browser → http://localhost:5000
```

**Step 2: Verify Live Data**
Dashboard should show:
- [ ] Real-time sensor readings
- [ ] Green dot for WiFi status (ONLINE)
- [ ] Updated timestamps
- [ ] Tabs for RFID and Attendance

**Step 3: Test Charts**
- [ ] Temperature & Humidity chart visible
- [ ] Sound & CO chart visible
- [ ] Data updating every 10 seconds

### Phase 6: RFID Testing

**Step 1: Get RFID Card UID**
```
1. Hold RFID card near RC522 reader
2. Check ESP32 Serial Monitor
3. Should show: "RFID Card Detected: 1A2B3C4D"
```

**Step 2: Register Card**
```bash
curl -X POST http://localhost:5000/api/rfid/register \
  -H "Content-Type: application/json" \
  -d '{"rfidUid":"1A2B3C4D","studentName":"Test Student"}'
```

**Step 3: Verify in Dashboard**
```
1. Go to http://localhost:5000 RFID Logs tab
2. Recent scan should appear in logs
3. Should show student name and timestamp
```

### Phase 7: Alert Testing

**Step 1: Trigger Sound Alert**
```
1. Make loud noise near sound sensor
2. Should trigger alert when > 80%
3. Check dashboard Alerts section
4. Alert appears with timestamp
```

**Step 2: Test Temperature Alert**
```
Can simulate by:
- Holding heat source near DHT22 (> 28°C)
Or
- Directly insert test value via API
```

**Step 3: Test CO Alert**
```
If available, test with CO source
Or verify JSON payload format for CO > 10 PPM
```

---

## 🐛 Verification Checklist

### ESP32 Firmware
- [ ] Serial output looks correct
- [ ] WiFi connects automatically
- [ ] Sensor readings displayed
- [ ] Data sent every 10 seconds
- [ ] LEDs blink appropriately
- [ ] RFID card detected with buzzer

### Node.js Server
- [ ] Server starts without errors
- [ ] Database created (data/classroom.db)
- [ ] Port 5000 accessible
- [ ] API health check returns OK
- [ ] No JavaScript errors in console
- [ ] Database queries successful

### Web Dashboard
- [ ] Loads without errors
- [ ] Real-time data updates visible
- [ ] Charts display correctly
- [ ] All tabs functional
- [ ] Responsive on mobile browsers
- [ ] No console errors (F12)

### Data Flow
- [ ] ESP32 → Server connection successful
- [ ] Server → Database storage working
- [ ] Database → Dashboard retrieval working
- [ ] RFID logs appearing in real-time
- [ ] Attendance records accumulating
- [ ] Alerts being created properly

---

## 🔄 Daily Operations

### Morning Startup
```bash
1. Start PC
2. cd [your-project-directory]
3. npm start
4. Open http://localhost:5000 in browser
5. Verify ESP32 is connected (status shows ONLINE)
```

### Monitor System
- Check for RED alerts in dashboard
- Review attendance records throughout day
- Verify sensor readings stay within normal range

### Daily Shutdown
```bash
1. Note daily statistics (export if needed)
2. Stop Node.js server (Ctrl+C)
3. Perform ESP32 rest if issues observed
```

---

## 🚨 Emergency Procedures

### System Not Responding
```bash
# Terminal 1: Restart server
npm start

# Terminal 2: Check ESP32 connection
curl http://localhost:5000/api/system/status

# If still offline: Reset ESP32 (press Reset button)
```

### Database Corruption
```bash
# Delete corrupted database
rm data/classroom.db

# Restart server (database auto-recreates)
npm start
```

### WiFi Disconnected
```
1. Check ESP32 Serial Monitor
2. Verify WiFi SSID is broadcast
3. Check password in a.ino
4. Reset ESP32 if necessary
```

---

## 📊 Performance Monitoring

### Check Server Status
```bash
curl http://localhost:5000/health
```

### View Recent Data
```bash
curl http://localhost:5000/api/sensor-data/latest
```

### Get Statistics
```bash
curl http://localhost:5000/api/sensor-stats
```

### Export Daily Report
```bash
curl http://localhost:5000/api/dashboard/export?format=csv&hours=24 > report.csv
```

---

## 🔒 Security Hardening (Production)

1. **Add API Authentication:**
   - Implement JWT tokens
   - Require API key for ESP32

2. **Use HTTPS:**
   - Add SSL certificates
   - Update API URLs

3. **Database Backup:**
   - Automate backups
   - Store off-site

4. **Access Control:**
   - Implement user roles
   - Restrict API endpoints

5. **Rate Limiting:**
   - Limit API requests
   - Prevent abuse

---

## 📝 Maintenance Schedule

| Task | Frequency | Action |
|------|-----------|--------|
| Check alerts | Daily | Review dashboard |
| Backup data | Weekly | Export CSV files |
| Update code | Monthly | Install npm updates |
| Calibrate sensors | Quarterly | Re-run diagnostics |
| Database cleanup | Quarterly | Archive old data |
| Full system test | Quarterly | Run test procedures |

---

**Last Updated:** February 28, 2026  
**Testing Complete! Your system is ready for deployment.** 🎉
