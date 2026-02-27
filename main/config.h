// Configuration File for Classroom Management System
// Edit these values based on your setup

// ==================== WiFi Configuration ====================
// Update these with your WiFi credentials
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// API Server Configuration
#define API_SERVER "http://YOUR_PC_IP:5000/api"
#define DEVICE_ID "CLASSROOM_01"  // Unique identifier for each ESP32

// ==================== Hardware PIN Mapping ====================
// Change these if you use different pins

// Analog Sensors
#define SOUND_SENSOR_PIN 35    // ADC pin for sound sensor
#define MQ7_PIN 32             // ADC pin for CO sensor

// Digital Sensors
#define DHT_PIN 25             // GPIO pin for DHT22
#define DHT_TYPE DHT22         // DHT sensor type

// RFID Module
#define RFID_SS_PIN 5          // Chip select pin
#define RFID_RST_PIN 27        // Reset pin

// Status Indicators
#define LED_WIFI_PIN 13        // Green WiFi indicator LED
#define LED_DATA_PIN 12        // Blue data transmission LED
#define BUZZER_PIN 26          // Audio buzzer

// ==================== SENSOR CALIBRATION ====================
// Adjust based on your environment and testing

// Sound Level
#define SOUND_THRESHOLD 80     // Alert threshold (0-100%)
#define SOUND_CALIBRATION 1.0  // Multiplier for calibration

// Temperature & Humidity
#define TEMP_OFFSET 0          // Temperature offset (if needed)
#define HUMIDITY_OFFSET 0      // Humidity offset (if needed)

// CO Sensor (MQ7)
#define MQ7_RO 10.0            // Calibration resistance value
#define MQ7_CALIBRATION 1.0    // PPM calculation multiplier

// ==================== ALERT THRESHOLDS ====================
// Adjust based on classroom requirements

// Temperature ranges (°C)
#define TEMP_HIGH_THRESHOLD 28
#define TEMP_LOW_THRESHOLD 18

// Humidity ranges (%)
#define HUMIDITY_HIGH_THRESHOLD 70
#define HUMIDITY_LOW_THRESHOLD 30

// Sound level (%)
#define SOUND_LEVEL_THRESHOLD 80

// CO concentration (PPM)
#define CO_LEVEL_THRESHOLD 10

// ==================== OPERATION PARAMETERS ====================
// Adjust these for system performance

// Data transmission interval (milliseconds)
#define SAMPLE_INTERVAL 10000   // Send data every 10 seconds

// WiFi connection attempts
#define WIFI_MAX_ATTEMPTS 20
#define WIFI_RETRY_INTERVAL 5000  // ms between retries

// RFID card timeout
#define RFID_CARD_TIMEOUT 5000  // ms - how long to remember last card

// Serial debug output
#define DEBUG_SERIAL 1          // 1 = enable, 0 = disable
#define SERIAL_BAUD 115200      // Serial monitor baud rate

// ==================== FEATURE FLAGS ====================
// Enable/disable features

#define ENABLE_SOUND_SENSOR 1   // Monitor sound level
#define ENABLE_DHT_SENSOR 1     // Monitor temp & humidity
#define ENABLE_MQ7_SENSOR 1     // Monitor CO levels
#define ENABLE_RFID_READER 1    // Read RFID cards
#define ENABLE_LED_INDICATORS 1 // Use status LEDs
#define ENABLE_BUZZER 1         // Audio feedback

// ==================== NETWORK SETTINGS ====================
// Advanced network configuration

#define WIFI_TIMEOUT 20         // seconds to wait for WiFi
#define API_TIMEOUT 5000        // ms timeout for API calls
#define API_RETRY_ATTEMPTS 3    // Number of API retry attempts

// ==================== CLASSROOM SETTINGS ====================
// Customize for your classroom

#define CLASSROOM_ID "CLASSROOM_01"
#define CLASSROOM_NAME "Main Classroom"
#define CLASSROOM_CAPACITY 30   // Maximum students
#define CLASS_START_TIME "08:00"
#define CLASS_END_TIME "16:00"

// ==================== TIMEZONE ====================
// Set your timezone
#define TIMEZONE_OFFSET 5.5     // Hours from UTC (e.g., +5:30 for India)
#define USE_DST 0               // Daylight saving time (1=yes, 0=no)

// ==================== OPTIONAL FEATURES ====================

// Email/SMS Alert (if you add support)
#define ENABLE_EMAIL_ALERTS 0
#define ALERT_EMAIL "admin@school.edu"

// Cloud integration (if you add support)
#define ENABLE_CLOUD_SYNC 0
#define CLOUD_SERVER "cloud.example.com"

// ==================== JSON PAYLOAD SIZE ====================
// If you get "not enough memory" errors, this can help
#define JSON_BUFFER_SIZE 256
