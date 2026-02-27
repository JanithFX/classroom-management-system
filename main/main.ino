/*
╔══════════════════════════════════════════════════════════════════════╗
║     CLASSROOM MANAGEMENT SYSTEM - ESP32 IoT FIRMWARE                 ║
║                  With Physical Pin Mapping                           ║
╚══════════════════════════════════════════════════════════════════════╝

ESP32 DEVKIT PHYSICAL PIN DIAGRAM:
═════════════════════════════════════════════════════════════════════════

                          ┌─────────────────┐
                          │   ESP32 DEVKIT  │
                          │   (TOP VIEW)    │
         Left Side (GND)  │                 │  Right Side (3.3V)
         ─────────────────┤                 ├─────────────────
           GND ─┬─────────┤ GND        3V3  ├────────┬─ 3V3
                 │         │                 │        │
               SDA   GND   │ GPIO27(RST)     │ GPIO26(BUZZER)
                D21 ──────┤       ▼         ├────── D26
                          │   RFID RC522    │
               MISO   D21  ├─ GPIO21(MISO)  ├─ GPIO25(DHT22DATA)
                          │   ▲ SPI Data    │
                          │   │ (I2C SDA)   │
               SCK    D23  ├─ GPIO23(SCK)   ├─ GPIO32(MQ7_ADC)
                          │               │
               MOSI   D19  ├─ GPIO19(MOSI)  ├─ GPIO35(SOUND_ADC)
                          │   ▼ SPI Data    │
                          │   │(I2C SCL)    │
         SDA (CS) D5  ────┤ GPIO5(SDA-CS)   ├─ GPIO2(BUILTIN_LED)
                          │               │
                       GND ├─ GPIO0        ├─ GPIO13(LED_WIFI)
                          └──┬─────────────┤
                             │ USB PWR     │
                             └─────────────┘

PIN ASSIGNMENT QUICK REFERENCE:
═════════════════════════════════════════════════════════════════════════
Analog Inputs (8-bit ADC):
  • GPIO35 (VP)  → Sound Sensor      (0-3.3V analog)
  • GPIO32 (SVP) → MQ7 CO Sensor     (0-3.3V analog)

Digital Inputs/Outputs:
  • GPIO25 → DHT22 Data (Temperature/Humidity)
  • GPIO13 → WiFi Status LED (Green indicator)
  • GPIO2 → Built-in LED (RFID & Data Feedback)

SPI Interface (for RFID RC522):
  • GPIO5  (D5)  → SDA (Chip Select) - RC522 CS pin
  • GPIO19 (D19) → MOSI (Master Out, Slave In) - Data send to RFID
  • GPIO21 (D21) → MISO (Master In, Slave Out) - Data receive from RFID
  • GPIO23 (D23) → SCK (Serial Clock) - Clock signal
  • GPIO27 (D27) → Reset (RST) - RFID module reset

RFID RC522 SPI CONNECTION (Important!):
═════════════════════════════════════════════════════════════════════════
  RC522 Module    →    ESP32 Pin
  ──────────────────────────────────
  VCC (+3.3V)    →    3V3 (with 100nF decoupling cap)
  GND            →    GND
  
  SDA (CS - Chip Select)  →    GPIO5 (D5)
  SCK (Serial Clock)      →    GPIO23 (D23)
  MOSI (Data In)          →    GPIO19 (D19)
  MISO (Data Out)         →    GPIO21 (D21)
  RST (Reset)             →    GPIO27 (D27)
  IRQ                     →    (Not used - leave disconnected)

═════════════════════════════════════════════════════════════════════════
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>

// ---- custom DHT22 reader (no external library required) ----
// Returns true on successful read, false otherwise.
// `pin` is the data pin attached to the DHT22 sensor.
bool readDHT22(uint8_t pin, float &temperature, float &humidity) {
  uint8_t data[5] = {0,0,0,0,0};
  uint32_t count = 0;

  // send start signal
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(1);
  digitalWrite(pin, HIGH);
  delayMicroseconds(30);
  pinMode(pin, INPUT_PULLUP);

  // wait for sensor response: 80µs LOW then 80µs HIGH
  count = 0;
  while (digitalRead(pin) == LOW) {
    if (++count >= 10000) return false;
  }
  count = 0;
  while (digitalRead(pin) == HIGH) {
    if (++count >= 10000) return false;
  }

  // read 40 bits
  for (int i = 0; i < 40; ++i) {
    // wait for low period (50µs)
    count = 0;
    while (digitalRead(pin) == LOW) {
      if (++count >= 10000) return false;
    }
    // measure high period length
    unsigned long t = micros();
    while (digitalRead(pin) == HIGH) {
      if (micros() - t > 100) break;
    }
    unsigned long duration = micros() - t;
    data[i/8] <<= 1;
    if (duration > 40) {
      data[i/8] |= 1;
    }
  }

  // checksum
  uint8_t checksum = data[0] + data[1] + data[2] + data[3];
  if (checksum != data[4]) return false;
  humidity = ((data[0] << 8) | data[1]) * 0.1;
  temperature = (((data[2] & 0x7F) << 8) | data[3]) * 0.1;
  if (data[2] & 0x80) temperature = -temperature;
  return true;
}

// ======================================== WIFI CREDENTIALS ============================================================
const char* ssid = "SSID";           // Change to your WiFi name
const char* password = "PW";   // Change to your WiFi password
const char* apiUrl = "http://SERVER-IP:5000/api"; // Change to your server IP

// ==================== TIMEZONE SETTINGS ====================
// Sri Lanka: UTC+5:30 (19800 seconds)
// No Daylight Saving Time (DST = 0)
const int timezone_offset = 19800;  // UTC+5:30 in seconds
const int dst_offset = 0;           // No DST

// ==================== PIN CONFIGURATION ====================
// ESP32 DEVKIT PIN MAPPING (Physical Board Labels)

// Sound Sensor (Analog Input)
#define SOUND_SENSOR_PIN 35  // Physical: VP (ADC0) - Analog sound detection

// DHT22 Temperature & Humidity (Digital Pin)
#define DHT_PIN 25           // Physical: D25 - DHT22 data line

// RFID RC522 (SPI Interface)
// SPI Configuration: D19(MOSI), D21(MISO), D23(SCK), D5(SDA-CS), D27(RST)
#define SS_PIN 5             // Physical: D5   - RFID SDA (Chip Select pin)
#define RST_PIN 27           // Physical: D27  - RFID Reset (RST)
// Note: MOSI=D19, MISO=D21, SCK=D23 (auto-mapped by SPI.begin())
MFRC522 rfid(SS_PIN, RST_PIN);

// MQ7 CO Sensor (Analog Input)
#define MQ7_PIN 32           // Physical: SVP (ADC1) - Analog CO detection

// LED Indicators & Buzzer (Digital Output)
#define LED_WIFI 13          // Physical: D13 - WiFi Status LED (Green)
#define LED_BUILTIN_PIN 2    // Physical: D2 - ESP32 Built-in LED (for RFID feedback)
// Note: Built-in LED is active-HIGH (digitalWrite(LED, HIGH) turns ON)

// ==================== SENSOR CALIBRATION ====================
const float MQ7_RO = 10.0;        // Calibration value for MQ7
const float SOUND_THRESHOLD = 2000; // Sound detection threshold
const unsigned long SAMPLE_INTERVAL = 10000;  // Send data every 10 seconds

// ==================== GLOBAL VARIABLES ====================
unsigned long lastSampleTime = 0;
String lastRFIDCard = "";
unsigned long lastRFIDTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== CLASSROOM MANAGEMENT SYSTEM - ESP32 ===");
  Serial.println("Initializing sensors...");
  
  // Pin setup
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  digitalWrite(LED_WIFI, LOW);
  digitalWrite(LED_BUILTIN_PIN, LOW);
  
  // Initialize sensors (DHT22 driver doesn't require a begin call)
  delay(500);
  
  // Initialize SPI for RFID
  // SPI.begin() auto-maps: MOSI(19), MISO(21), SCK(23)
  // Manual CS(5) and RST(27) configuration via MFRC522 constructor
  Serial.println("Initializing SPI bus...");
  SPI.begin(
    23,  // SCK  (Serial Clock) - GPIO23 / D23
    21,  // MISO (Master In, Slave Out) - GPIO21 / D21 / SDA
    19,  // MOSI (Master Out, Slave In) - GPIO19 / D19
    5    // SS   (Chip Select) - GPIO5 / D5
  );
  
  Serial.println("Initializing RFID RC522...");
  rfid.PCD_Init();
  delay(500);
  
  // Connect to WiFi
  connectToWiFi();
  
  // Synchronize time with NTP server (Sri Lankan Time: UTC+5:30)
  Serial.println("Synchronizing time with NTP (Sri Lanka - UTC+5:30)...");
  
  // Sri Lanka timezone: UTC+5:30 (5*3600 + 30*60 = 19800 seconds)
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov");
  
  // Wait for time to be set
  time_t now = time(nullptr);
  int attempts = 0;
  while (now < 24 * 3600 && attempts < 60) {  // Increased timeout to 60 attempts
    delay(500);
    Serial.print(".");
    now = time(nullptr);
    attempts++;
  }
  Serial.println();
  
  if (now > 24 * 3600) {
    Serial.println("✓ Time synchronized successfully!");
    Serial.print("Current time (Sri Lanka): ");
    Serial.println(getTimestamp());
  } else {
    Serial.println("⚠ Warning: Time sync in progress, continuing anyway...");
    Serial.print("Current time: ");
    Serial.println(getTimestamp());
  }
  
  Serial.println("System ready!");
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_WIFI, LOW);
    connectToWiFi();
  } else {
    digitalWrite(LED_WIFI, HIGH);
  }
  
  // Read RFID continuously
  if (rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {
      handleRFIDCard();
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
  
  // Send sensor data at intervals
  if (millis() - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = millis();
    collectAndSendData();
  }
  
  delay(100);
}

// ==================== WiFi Connection ====================
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_WIFI, HIGH);
  } else {
    Serial.println("\nWiFi Failed!");
  }
}

// ==================== SENSOR READING ====================
float readSoundLevel() {
  int rawValue = analogRead(SOUND_SENSOR_PIN);
  float soundLevel = (rawValue / 4095.0) * 100;  // Convert to percentage
  return soundLevel;
}

void readTemperatureHumidity(float &temp, float &humidity) {
  // Try reading DHT22 multiple times with delays
  int maxRetries = 3;
  bool success = false;
  
  for (int attempt = 1; attempt <= maxRetries; attempt++) {
    if (readDHT22(DHT_PIN, temp, humidity)) {
      success = true;
      break;
    }
    if (attempt < maxRetries) {
      delay(1000);  // Wait 1 second before retry
    }
  }
  
  if (!success) {
    temp = 0;
    humidity = 0;
    Serial.println("⚠ DHT22 Read Failed - Check sensor connection on GPIO25");
  }
}

float readCOLevel() {
  int rawValue = analogRead(MQ7_PIN);
  // Convert ADC value to PPM using calibration
  float voltage = (rawValue / 4095.0) * 3.3;
  float CO_ppm = (voltage / 3.3) * 1000;  // Simple conversion (adjust based on calibration)
  return CO_ppm;
}

// ==================== RFID HANDLING ====================
// RFID RC522 Communication via SPI:
// When card detected → RC522 pulls data via SPI GPIO19(MOSI), GPIO21(MISO), GPIO23(SCK)
// This function processes the received UID from the RFID card

// Send scanned RFID to server for real-time dashboard capture
void sendRFIDScan(const String &uid) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, cannot send RFID scan");
    return;
  }

  HTTPClient http;
  String url = String(apiUrl) + "/rfid/scan";
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<128> scanDoc;
  scanDoc["rfidUid"] = uid;
  String payload;
  serializeJson(scanDoc, payload);

  int httpCode = http.POST(payload);
  if (httpCode >= 200 && httpCode < 300) {
    Serial.println("RFID scan sent to server");
  } else {
    Serial.print("Failed to send RFID scan, HTTP code: ");
    Serial.println(httpCode);
  }
  http.end();
}

// helper: send a log message to the server whenever a card is scanned
void sendAttendanceLog(const String &uid, const String &action = "ENTRY") {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = String(apiUrl) + "/rfid/log";
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<128> logDoc;
  logDoc["rfidUid"] = uid;
  logDoc["action"] = action;
  String payload;
  serializeJson(logDoc, payload);

  int httpCode = http.POST(payload);
  if (httpCode >= 200 && httpCode < 300) {
    Serial.println("RFID attendance logged on server");
  } else {
    Serial.print("Failed to send RFID log, HTTP code: ");
    Serial.println(httpCode);
  }
  http.end();
}


void handleRFIDCard() {
  String UID = "";
  // Read UID bytes via SPI MISO line (GPIO21)
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) UID += "0";
    UID += String(rfid.uid.uidByte[i], HEX);
  }
  UID.toUpperCase();
  
  Serial.print("RFID Card Detected (via GPIO5 SDA-CS, GPIO23 SCK, GPIO21 MISO): ");
  Serial.println(UID);
  
  // Trigger feedback: Built-in LED (GPIO2) blink
  digitalWrite(LED_BUILTIN_PIN, HIGH);    // Built-in LED on
  delay(200);
  digitalWrite(LED_BUILTIN_PIN, LOW);     // Built-in LED off
  delay(100);
  digitalWrite(LED_BUILTIN_PIN, HIGH);    // Built-in LED on again
  delay(200);
  digitalWrite(LED_BUILTIN_PIN, LOW);     // Built-in LED off
  
  // Store last RFID for sending in next data packet
  lastRFIDCard = UID;
  lastRFIDTime = millis();

  // Send RFID scan to server immediately (for real-time dashboard registration)
  sendRFIDScan(UID);
  
  // Also post an attendance log to the server
  sendAttendanceLog(UID);
}

// ==================== DATA COLLECTION & TRANSMISSION ====================
void collectAndSendData() {
  // Read all sensors
  float soundLevel = readSoundLevel();
  float temperature, humidity;
  readTemperatureHumidity(temperature, humidity);
  float coLevel = readCOLevel();
  
  Serial.println("\n--- Sensor Readings ---");
  Serial.print("Sound Level: ");
  Serial.print(soundLevel);
  Serial.println("%");
  
  if (temperature > 0 || humidity > 0) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  } else {
    Serial.println("Temperature: -- (DHT22 error)");
    Serial.println("Humidity: -- (DHT22 error)");
  }
  
  Serial.print("CO Level: ");
  Serial.print(coLevel);
  Serial.println(" PPM");
  
  // Create JSON payload
  StaticJsonDocument<256> doc;
  doc["deviceId"] = "CLASSROOM_01";
  doc["soundLevel"] = soundLevel;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["coLevel"] = coLevel;
  doc["rfidCard"] = lastRFIDCard;
  doc["timestamp"] = getTimestamp();
  doc["rssi"] = WiFi.RSSI();  // Signal strength
  
  // Serialize JSON
  String jsonData;
  serializeJson(doc, jsonData);
  
  // Send to API
  sendDataToAPI(jsonData);
  
  // Clear RFID for next reading
  if (millis() - lastRFIDTime > 5000) {
    lastRFIDCard = "";
  }
}

void sendDataToAPI(String jsonData) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(apiUrl) + "/sensor-data";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(jsonData);
    
    if (httpCode == 200 || httpCode == 201) {
      Serial.println("Data sent successfully!");
      digitalWrite(LED_BUILTIN_PIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN_PIN, LOW);
    } else {
      Serial.print("Error sending data. HTTP code: ");
      Serial.println(httpCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}

// ==================== UTILITIES ====================
String getTimestamp() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}
