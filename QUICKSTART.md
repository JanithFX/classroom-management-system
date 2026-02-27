# 🚀 Quick Start Guide - 5 Minutes to Running

---

## Prerequisites
- ESP32 microcontroller with all sensors connected
- Node.js (LTS version) installed on your PC
- Arduino IDE (latest version)
- USB cable for ESP32 programming
- WiFi network (2.4GHz)

---

## Step 1: Upload ESP32 Firmware (2 minutes)

### In Arduino IDE:
1. Open `a.ino` file
2. Edit lines 11-13:
   ```cpp
   const char* ssid = "YOUR_WIFI"; 
   const char* password = "YOUR_WIFI_PASSWORD";
   const char* apiUrl = "http://192.168.1.X:5000/api";  // Your PC IP
   ```
3. Click **Upload** (Ctrl+U)
4. Wait for "Done uploading" message
5. Open Serial Monitor (Ctrl+Shift+M) - should show:
   ```
   Connecting to WiFi...
   WiFi Connected!
   System ready!
   ```

---

## Step 2: Start Backend Server (1 minute)

### In Command Prompt/Terminal:
```bash
# Navigate to your project directory
cd [your-project-directory]
npm install
npm start
```

You should see:
```
Classroom Management API Server running on port 5000
Database connected
Dashboard available at http://localhost:5000
```

---

## Step 3: Open Web Dashboard (0.5 minutes)

Open your browser:
```
http://localhost:5000
```

You should see the dashboard with real-time sensor data! 🎉

---

## Step 4: Register RFID Cards (1.5 minutes)

Get a RFID card UID from Serial Monitor:
- Hold card to RC522 reader
- Serial Monitor shows: `RFID Card Detected: 1A2B3C4D`

Register it:
```bash
curl -X POST http://localhost:5000/api/rfid/register \
  -H "Content-Type: application/json" \
  -d '{"rfidUid": "1A2B3C4D", "studentName": "John Doe"}'
```

---

## 🎯 What Works Now

✅ Real-time sensor readings  
✅ Temperature & humidity monitoring  
✅ Sound level detection  
✅ CO concentration tracking  
✅ RFID attendance logging  
✅ Live dashboard with charts  
✅ Alert system  
✅ Data export  

---

## 🔧 Troubleshooting

### ESP32 won't connect to WiFi?
- Check WiFi SSID and password are correct
- Ensure 2.4GHz WiFi (not 5GHz)
- Look at Serial Monitor errors

### No data in dashboard?
- Is backend running? Check terminal
- Is ESP32 connected to WiFi? Check Serial Monitor
- Same network? Ping ESP32's IP from PC

### Sensors not reading?
- Run `sensor_test.ino` to diagnose
- Check wiring matches diagram in README.md

### Port 5000 already in use?
```bash
npm start -- --port 5001
```

---

## 📚 Next Steps

1. **Read README.md** - Full documentation and wiring diagrams
2. **Customize thresholds** - Edit alert thresholds in `routes/sensors.js`
3. **Add more cards** - Register all student RFID cards
4. **Monitor attendance** - Check attendance tab in dashboard
5. **Export data** - Download CSV from API: `/api/dashboard/export?format=csv`

---

## 🆘 Need Help?

Check the `README.md` file for:
- Complete wiring diagrams
- Library installation instructions
- API endpoint reference
- Alert threshold details
- Advanced configuration

---

## 📊 System Status Indicators

**Green LED (GPIO13):** WiFi connected  
**Blue LED (GPIO12):** Data transferred  
**Buzzer (GPIO26):** RFID card detected  

---

**Last Updated:** February 28, 2026  
**Congratulations! Your classroom management system is now running!** 🎓
