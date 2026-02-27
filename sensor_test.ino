// Classroom Management System - Sensor Test & Diagnostic Sketch
// Use this to test individual sensors before deploying the main system

#include <SPI.h>
#include <MFRC522.h>
#include "DHT.h"

// ==================== PIN CONFIGURATION ====================
#define SOUND_SENSOR_PIN 35
#define DHT_PIN 25
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

#define SS_PIN 5
#define RST_PIN 27
MFRC522 rfid(SS_PIN, RST_PIN);

#define MQ7_PIN 32
#define LED_WIFI 13
#define LED_DATA 12

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n========================================");
  Serial.println("Classroom Management System - DIAGNOSTICS");
  Serial.println("========================================\n");
  
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_DATA, OUTPUT);
  digitalWrite(LED_WIFI, LOW);
  digitalWrite(LED_DATA, LOW);
  
  // Test sound sensor
  Serial.println("Testing Sound Sensor on GPIO35...");
  testSoundSensor();
  delay(1000);
  
  // Test DHT22
  Serial.println("\nInitializing DHT22 on GPIO25...");
  dht.begin();
  delay(500);
  testDHTSensor();
  delay(1000);
  
  // Test MQ7
  Serial.println("\n\nTesting MQ7 CO Sensor on GPIO32...");
  testMQ7Sensor();
  delay(1000);
  
  // Test RFID
  Serial.println("\n\nInitializing RFID RC522...");
  SPI.begin();
  rfid.PCD_Init();
  testRFIDSensor();
  
  Serial.println("\n\n========================================");
  Serial.println("Diagnostic Complete!");
  Serial.println("========================================");
}

void loop() {
  // Continuous monitoring during diagnostic
  Serial.println("\n--- Continuous Sensor Reading ---");
  
  float sound = analogRead(SOUND_SENSOR_PIN) / 40.95;
  Serial.print("Sound Level: ");
  Serial.print(sound, 2);
  Serial.println("% (0-100)");
  
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temp, 2);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidity, 2);
  Serial.println("%");
  
  int coRaw = analogRead(MQ7_PIN);
  float co_ppm = (coRaw / 4095.0) * 1000;
  Serial.print("CO Level (Raw): ");
  Serial.print(coRaw);
  Serial.print(" (PPM: ");
  Serial.print(co_ppm, 2);
  Serial.println(")");
  
  // RFID continuous check
  if (rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {
      digitalWrite(LED_DATA, HIGH);
      Serial.print("RFID Card Detected: ");
      for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println();
      digitalWrite(LED_DATA, LOW);
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
  
  delay(2000);
}

// ==================== SENSOR TEST FUNCTIONS ====================

void testSoundSensor() {
  Serial.print("Reading analog values (10 samples): ");
  for (int i = 0; i < 10; i++) {
    int value = analogRead(SOUND_SENSOR_PIN);
    float percent = (value / 4095.0) * 100;
    Serial.print(percent, 1);
    Serial.print("% ");
    delay(100);
  }
  Serial.println("\nSound Sensor: OK ✓");
}

void testDHTSensor() {
  delay(2000);  // DHT needs time to stabilize
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 Sensor: ERROR ✗");
    Serial.println("  - Check power connections");
    Serial.println("  - Verify GPIO25 data line");
    Serial.println("  - Try adding 4.7kΩ pullup resistor");
    return;
  }
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.println("DHT22 Sensor: OK ✓");
}

void testMQ7Sensor() {
  Serial.print("Reading analog values (10 samples): ");
  int total = 0;
  for (int i = 0; i < 10; i++) {
    int value = analogRead(MQ7_PIN);
    Serial.print(value);
    Serial.print(" ");
    total += value;
    delay(100);
  }
  int avg = total / 10;
  float volts = (avg / 4095.0) * 3.3;
  float ppm = (volts / 3.3) * 1000;
  
  Serial.println();
  Serial.print("Average Raw: ");
  Serial.println(avg);
  Serial.print("Voltage: ");
  Serial.print(volts, 3);
  Serial.println("V");
  Serial.print("CO Estimation: ");
  Serial.print(ppm, 2);
  Serial.println(" PPM");
  
  if (avg > 100) {
    Serial.println("MQ7 CO Sensor: OK ✓");
  } else {
    Serial.println("MQ7 CO Sensor: Check connections ⚠");
  }
}

void testRFIDSensor() {
  uint8_t v = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  
  Serial.print("RFID Firmware Version: 0x");
  Serial.println(v, HEX);
  
  if ((v & 0xB0) == 0xB0 || v == 0x88 || v == 0x90 || v == 0x91) {
    Serial.println("RFID RC522: OK ✓");
    Serial.println("Waiting for card... (place card near reader)");
  } else {
    Serial.println("RFID RC522: NOT DETECTED ✗");
    Serial.println("  - Check SPI connections (MISO, MOSI, SCK)");
    Serial.println("  - Verify SS pin (GPIO5) and RST pin (GPIO27)");
    Serial.println("  - Check 3.3V power supply");
    Serial.println("Firmware Version found: 0x");
    Serial.println(v, HEX);
  }
}

// ==================== LED TEST ====================
void testLEDs() {
  Serial.println("\nTesting LEDs...");
  digitalWrite(LED_WIFI, HIGH);
  Serial.println("WiFi LED ON");
  delay(500);
  digitalWrite(LED_WIFI, LOW);
  delay(500);
  
  digitalWrite(LED_DATA, HIGH);
  Serial.println("Data TX LED ON");
  delay(500);
  digitalWrite(LED_DATA, LOW);
  delay(500);
  
  Serial.println("LED Test: OK ✓");
}
