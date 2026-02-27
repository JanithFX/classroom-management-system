# API Documentation - Classroom Management System

## Base URL
```
http://localhost:5000/api
```

---

## 📡 Sensor Endpoints

### POST `/sensor-data`
**Description:** ESP32 sends sensor readings to the server

**Request Body:**
```json
{
  "deviceId": "CLASSROOM_01",
  "soundLevel": 45.2,
  "temperature": 24.5,
  "humidity": 55.3,
  "coLevel": 2.1,
  "rfidCard": "1A2B3C4D",
  "timestamp": "2026-02-24 10:30:45",
  "rssi": -67
}
```

**Response:**
```json
{
  "success": true,
  "message": "Sensor data recorded",
  "data": {
    "deviceId": "CLASSROOM_01",
    "timestamp": "2026-02-24 10:30:45"
  }
}
```

---

### GET `/sensor-data/latest`
**Description:** Get the most recent sensor reading

**Response:**
```json
{
  "id": 1234,
  "deviceId": "CLASSROOM_01",
  "soundLevel": 45.2,
  "temperature": 24.5,
  "humidity": 55.3,
  "coLevel": 2.1,
  "rssi": -67,
  "timestamp": "2026-02-24 10:30:45"
}
```

---

### GET `/sensor-data/history`
**Description:** Get historical sensor data

**Query Parameters:**
- `limit` (optional): Number of records (default: 100)

**Example:**
```
GET /sensor-data/history?limit=50
```

**Response:**
```json
[
  {
    "id": 1234,
    "deviceId": "CLASSROOM_01",
    "soundLevel": 45.2,
    "temperature": 24.5,
    "humidity": 55.3,
    "coLevel": 2.1,
    "rssi": -67,
    "timestamp": "2026-02-24 10:30:45"
  },
  ...
]
```

---

### GET `/sensor-stats`
**Description:** Get 1-hour statistics for all sensors

**Response:**
```json
{
  "avgSound": 42.5,
  "maxSound": 87.3,
  "avgTemp": 24.2,
  "maxTemp": 26.1,
  "minTemp": 22.3,
  "avgHumidity": 54.8,
  "avgCO": 2.3,
  "maxCO": 5.1,
  "totalReadings": 6
}
```

---

## 🏷️ RFID Endpoints

### POST `/rfid/register`
**Description:** Register a new RFID card for a student

**Request Body:**
```json
{
  "rfidUid": "1A2B3C4D",
  "studentName": "John Doe"
}
```

**Response:**
```json
{
  "success": true,
  "message": "RFID card registered",
  "cardId": "550e8400-e29b-41d4-a716-446655440000",
  "rfidUid": "1A2B3C4D",
  "studentName": "John Doe"
}
```

---

### POST `/rfid/log`
**Description:** Log RFID card access (entry/exit)

**Request Body:**
```json
{
  "rfidUid": "1A2B3C4D",
  "action": "ENTRY"
}
```

**Response:**
```json
{
  "success": true,
  "message": "Access logged",
  "cardId": "550e8400-e29b-41d4-a716-446655440000",
  "studentName": "John Doe",
  "action": "ENTRY",
  "timestamp": "2026-02-24 10:30:45"
}
```

---

### GET `/rfid/cards`
**Description:** Get all registered RFID cards

**Response:**
```json
[
  {
    "cardId": "550e8400-e29b-41d4-a716-446655440000",
    "studentName": "John Doe",
    "rfidUid": "1A2B3C4D",
    "createdAt": "2026-02-24 10:00:00"
  },
  ...
]
```

---

### GET `/rfid/logs`
**Description:** Get RFID access logs

**Query Parameters:**
- `limit` (optional): Number of logs (default: 50)

**Response:**
```json
[
  {
    "id": 1,
    "timestamp": "2026-02-24 10:30:45",
    "action": "ENTRY",
    "studentName": "John Doe",
    "rfidUid": "1A2B3C4D"
  },
  ...
]
```

---

### GET `/rfid/attendance`
**Description:** Get today's attendance records

**Response:**
```json
[
  {
    "studentName": "John Doe",
    "rfidUid": "1A2B3C4D",
    "entries": 2,
    "firstEntry": "2026-02-24 08:30:15",
    "lastEntry": "2026-02-24 12:30:45"
  },
  ...
]
```

---

### DELETE `/rfid/cards/:cardId`
**Description:** Remove a registered RFID card

**Example:**
```
DELETE /rfid/cards/550e8400-e29b-41d4-a716-446655440000
```

**Response:**
```json
{
  "success": true,
  "message": "RFID card deleted"
}
```

---

## 📊 Dashboard Endpoints

### GET `/dashboard/summary`
**Description:** Get dashboard overview with latest data

**Response:**
```json
{
  "latestSensor": {
    "soundLevel": 45.2,
    "temperature": 24.5,
    "humidity": 55.3,
    "coLevel": 2.1,
    "timestamp": "2026-02-24 10:30:45"
  },
  "attendance": 28,
  "activeAlerts": 2,
  "recentAlerts": [
    {
      "id": 1,
      "type": "TEMPERATURE",
      "severity": "WARNING",
      "message": "High temperature: 28.5°C",
      "timestamp": "2026-02-24 10:25:00",
      "resolved": 0
    }
  ],
  "stats": {
    "avgSound": 42.5,
    "avgTemp": 24.2,
    "avgHumidity": 54.8,
    "avgCO": 2.3
  }
}
```

---

### GET `/alerts`
**Description:** Get system alerts with filtering

**Query Parameters:**
- `type` (optional): Filter by alert type (SOUND, TEMPERATURE, HUMIDITY, CO_LEVEL)
- `resolved` (optional): 0=unresolved, 1=resolved (default: 0)
- `limit` (optional): Number of alerts (default: 100)

**Example:**
```
GET /alerts?type=TEMPERATURE&resolved=0&limit=10
```

**Response:**
```json
[
  {
    "id": 1,
    "type": "TEMPERATURE",
    "severity": "WARNING",
    "message": "High temperature: 28.5°C",
    "value": 28.5,
    "timestamp": "2026-02-24 10:25:00",
    "resolved": 0
  },
  ...
]
```

---

### PUT `/alerts/:alertId/resolve`
**Description:** Mark an alert as resolved

**Example:**
```
PUT /alerts/1/resolve
```

**Response:**
```json
{
  "success": true,
  "message": "Alert resolved"
}
```

---

### GET `/system/status`
**Description:** Get current system status

**Response:**
```json
{
  "status": "ONLINE",
  "lastReading": "2026-02-24 10:30:45",
  "minutesSinceLastReading": 0,
  "timestamp": "2026-02-24 10:30:46"
}
```

---

### GET `/dashboard/chart-data`
**Description:** Get chart data for dashboard visualization

**Query Parameters:**
- `hours` (optional): Time period in hours (default: 24)

**Example:**
```
GET /dashboard/chart-data?hours=24
```

**Response:**
```json
{
  "tempHumidData": [
    {
      "hour": "2026-02-24 08:00:00",
      "avgTemp": 24.1,
      "avgHumidity": 55.2,
      "maxTemp": 25.3,
      "minTemp": 23.0
    },
    ...
  ],
  "pollutantData": [
    {
      "hour": "2026-02-24 08:00:00",
      "avgSound": 42.3,
      "avgCO": 2.1
    },
    ...
  ],
  "period": "24 hours"
}
```

---

### GET `/dashboard/export`
**Description:** Export sensor data as CSV or JSON

**Query Parameters:**
- `format` (optional): "csv" or "json" (default: csv)
- `hours` (optional): Time period in hours (default: 24)

**Example:**
```
GET /dashboard/export?format=csv&hours=24
```

**CSV Response:**
```
Timestamp,Device ID,Sound Level,Temperature,Humidity,CO Level,Signal Strength
2026-02-24 10:30:45,CLASSROOM_01,45.2,24.5,55.3,2.1,-67
2026-02-24 10:29:45,CLASSROOM_01,44.8,24.3,55.1,2.0,-68
```

---

## ❤️ Health Check

### GET `/health`
**Description:** Check if API server is running

**Response:**
```json
{
  "status": "OK",
  "timestamp": "2026-02-24 10:30:45"
}
```

---

## 🔑 Error Responses

All endpoints return error responses in this format:

```json
{
  "error": "Error message",
  "message": "Detailed error description (if available)"
}
```

### Common HTTP Status Codes
- `200` - Success (GET/PUT)
- `201` - Created (POST)
- `400` - Bad Request
- `404` - Not Found
- `500` - Server Error

---

## 📝 Usage Examples

### Using cURL

**Get Latest Sensor Data:**
```bash
curl http://localhost:5000/api/sensor-data/latest
```

**Register RFID Card:**
```bash
curl -X POST http://localhost:5000/api/rfid/register \
  -H "Content-Type: application/json" \
  -d '{"rfidUid":"1A2B3C4D","studentName":"John Doe"}'
```

**Get Today's Attendance:**
```bash
curl http://localhost:5000/api/rfid/attendance
```

**Export Data:**
```bash
curl http://localhost:5000/api/dashboard/export?format=csv&hours=24 > data.csv
```

---

### Using JavaScript/Fetch

```javascript
// Get latest sensor data
fetch('/api/sensor-data/latest')
  .then(r => r.json())
  .then(data => console.log(data));

// Register RFID card
fetch('/api/rfid/register', {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' },
  body: JSON.stringify({
    rfidUid: '1A2B3C4D',
    studentName: 'John Doe'
  })
})
.then(r => r.json())
.then(data => console.log(data));
```

---

## 🔒 Security Notes

- All endpoints currently accept requests from any origin
- Consider implementing API authentication for production
- Use HTTPS in production environments
- Validate all input parameters
- Implement rate limiting for API endpoints

---

**Last Updated:** February 28, 2026
