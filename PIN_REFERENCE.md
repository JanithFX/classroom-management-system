# ESP32 Physical Pin Reference - Classroom Management System

## 📌 Quick Pin Lookup

### All GPIO Pins Used in This Project

```
ESP32 DEV BOARD - PIN LAYOUT (Both Sides)

LEFT SIDE                               RIGHT SIDE
═════════════════════════════════════════════════════════
GND         │                          │  3V3 (Power)
IO34        │                          │  IO35 - SOUND (VP)
IO35 - SOUND│  ┌─────────────────────┐ │  EN (Enable)
IO32 - MQ7  │  │                     │ │  IO36 (Sensor VP)
IO33        │  │   ESP32 DevKit      │ │  IO39
IO25 - DHT  │  │                     │ │  IO34
IO26 - BUZ  │  │     Board           │ │  IO35 - SOUND *
IO27 - RST  │  │    (top view)       │ │  GND
IO14        │  │                     │ │  IO32 - MQ7 *
IO12 - LED  │  │                     │ │  IO33
IO13 - LED  │  │                     │ │  IO25 - DHT *
SD2         │  │                     │ │  IO26 - BUZ *
SD3         │  │                     │ │  IO27 - RST *
CMD         │  │                     │ │  IO14
CLK         │  └─────────────────────┘ │  IO12 - LED *
GND         │                          │  GND
5V          │                          │  IO13 - LED *
3V3         │                          │  D1 (TX)
EN          │                          │  D0 (RX)
═════════════════════════════════════════════════════════
* = Pins used in this project
```

## 🔌 Pinout Detail Table

### Input/Output Summary

| # | GPIO | Physical | Physical Name | Connection | Type | Function |
|---|------|----------|---|---|---|---|
| 1 | 34 | Left-4 | ADC6 | Not used | Input | - |
| 2 | **35** | Left-5 | **ADC0/VP** | **Sound Sensor** | **Input (ADC)** | **Noise detection** |
| 3 | 36 | Right-3 | SENSOR_VP | Not used | Input | - |
| 4 | 39 | Right-4 | SENSOR_VN | Not used | Input | - |
| 5 | **32** | Right-8 | **ADC1/SVP** | **MQ7 Sensor** | **Input (ADC)** | **CO detection** |
| 6 | 33 | Right-7 | IO33 | Not used | GPIO | - |
| 7 | **25** | Left-13 | **D25** | **DHT22 Data** | **GPIO** | **Temperature/Humidity** |
| 8 | **26** | Left-12 | **D26** | **Buzzer** | **GPIO (Out)** | **Audio alert** |
| 9 | **27** | Left-11 | **D27** | **RFID RST** | **GPIO (Out)** | **RFID Reset** |
| 10 | 14 | Left-10 | IO14 | Not used | GPIO | - |
| 11 | **12** | Left-9 | **D12** | **LED Data (Blue)** | **GPIO (Out)** | **TX indicator** |
| 12 | **13** | Left-8 | **D13** | **LED WiFi (Green)** | **GPIO (Out)** | **WiFi status** |
| 13 | 2 | Left-7 | IO2 | Not used | GPIO | - |
| 14 | 4 | Left-6 | IO4 | Not used | GPIO | - |
| 15 | **5** | Left-15 | **D5** | **RFID CS/SS** | **GPIO (Out)** | **RFID Chip Select** |
| 16 | 18 | SPI | -  | Not exposed separately | GPIO | - |
| 17 | **19** | SPI Bus | **D19** | **RFID MOSI** | **GPIO (Out)** | **SPI send to RFID** |
| 18 | 21 | SPI Bus | **D21** | **RFID MISO** | **GPIO (In)** | **SPI receive from RFID** |
| 19 | **23** | SPI Bus | **D23** | **RFID SCK** | **GPIO (Out)** | **SPI Clock** |
| 20 | GND | *Multiple* | GND | Ground | Ground | All GND common |
| 21 | 3V3 | Top/Right | 3V3 | RFID +3.3V | Power | RFID power supply |
| 22 | 5V | Top | 5V | USB Power | Power | USB power (optional) |

---

## 🎯 By Function Grouping

### Sensors (Read Data)

```
ANALOG INPUTS (ADC):
├─ GPIO35 (VP)    ← Sound Sensor       (0-4095 → 0-100%)
└─ GPIO32 (SVP)   ← MQ7 CO Sensor      (0-4095 → 0-500+ PPM)

DIGITAL INPUTS:
└─ GPIO25 (D25)   ← DHT22 Data Line    (Temperature & Humidity)
```

### Outputs (Control)

```
LED INDICATORS:
├─ GPIO13 (D13)   → WiFi Status (Green)
└─ GPIO12 (D12)   → Data TX (Blue)

AUDIO:
└─ GPIO26 (D26)   → Buzzer

RFID CONTROL:
├─ GPIO5 (D5)     → CS/Chip Select (Active LOW)
└─ GPIO27 (D27)   → RFID Reset (Pull LOW to reset)
```

### SPI Bus (Shared - RFID)

```
SPI INTERFACE (Shared Bus for multiple devices possible):
├─ GPIO19 (D19)   → MOSI (Master Out, Slave In)  [Output]
├─ GPIO21 (D21)   → MISO (Master In, Slave Out)  [Input]  ← Also I2C SDA
├─ GPIO23 (D23)   → SCK (Serial Clock)           [Output] ← Also I2C SCL
└─ GPIO5 (D5)     → CS/SS (Chip Select)          [Output] [Per-device]
```

---

## 📋 Pin Configuration Code Reference

### In a.ino (Lines 85-108)

```cpp
#define SOUND_SENSOR_PIN 35     // Physical: VP (ADC0)
#define DHT_PIN 25              // Physical: D25
#define SS_PIN 5                // Physical: D5 (RFID CS)
#define RST_PIN 27              // Physical: D27 (RFID Reset)
#define MQ7_PIN 32              // Physical: SVP (ADC1)
#define LED_WIFI 13             // Physical: D13
#define LED_DATA 12             // Physical: D12
#define BUZZER_PIN 26           // Physical: D26
```

### SPI Initialization Code (Lines 143-149)

```cpp
SPI.begin(
  23,  // SCK  - Physical: D23
  21,  // MISO - Physical: D21 (SDA equivalent)
  19,  // MOSI - Physical: D19
  5    // SS   - Physical: D5 (CS)
);
rfid.PCD_Init();  // Initialize RC522 on CS/RST pins
```

---

## 🔗 RFID RC522 Connection (Critical!)

### Pin Mapping for RC522 Module

```
RC522 Module       →    ESP32 Board Pin    →    Physical Label
─────────────────────────────────────────────────────────────
VCC (3.3V)    →    3V3                 →    Power Rail (top-right)
GND           →    GND (multiple)      →    Ground Rail (bottom)
RST           →    GPIO27              →    D27 (left-11)
CS/SDA        →    GPIO5               →    D5  (left-15)
MOSI          →    GPIO19              →    D19 (SPI Bus)
SCK           →    GPIO23              →    D23 (SPI Bus)
MISO          →    GPIO21              →    D21 (SPI Bus)
IRQ           →    (not used)          →    Leave disconnected
```

### RFID Wiring Checklist

- [ ] RC522 VCC → ESP32 3V3 (with 100nF capacitor to GND)
- [ ] RC522 GND → ESP32 GND
- [ ] RC522 RST → GPIO27 (D27) - Reset pin
- [ ] RC522 CS → GPIO5 (D5) - Chip Select
- [ ] RC522 MOSI → GPIO19 (D19) - Data to RFID
- [ ] RC522 MISO → GPIO21 (D21) - Data from RFID
- [ ] RC522 SCK → GPIO23 (D23) - Clock signal
- [ ] RC522 IRQ → (leave floating/disconnected)

---

## ⚡ Power Supply Notes

### RFID RC522 Power

```
3V3 Power Rail:
├─ Source: ESP32 3V3 output (500mA typical)
├─ RFID Current Draw: ~100mA typical
├─ Capacitor: 100nF (0.1µF) near VCC/GND pins
└─ Protection: Check power supply can handle all devices
```

### USB Power

- USB gives ~5V to ESP32 boot circuit
- 3V3 generated on-board by LDO regulator
- Sufficient for all sensors + RFID in this design

---

## 🔄 SPI Bus Sharing (Important!)

Current configuration uses **dedicated CS pin** for RFID:
- GPIO5 = Chip Select (Active LOW)
- GPIO19, GPIO21, GPIO23 = Shared data lines

**To add more SPI devices:**
1. Use different CS pin for each device
2. Connect all to same MISO/MOSI/SCK lines
3. Modify SPI.begin() if using different pins

---

## 🎨 Physical Pin Colors (Standard)

```
Typical ESP32 Dev Board Color Coding:
├─ Red traces    → 5V power
├─ Orange traces → 3V3 power
├─ Black traces  → GND (common)
├─ Blue traces   → GPIO signals
└─ Green traces  → Special pins (RST, EN, etc.)
```

---

## ✅ Validation Checklist

Before uploading firmware:
- [ ] All 8 active GPIO pins labeled in code
- [ ] SPI pins (19, 21, 23) configured in SPI.begin()
- [ ] ADC pins (35, 32) set to analog mode
- [ ] Chip Select (5) and Reset (27) set to output mode
- [ ] All comments include physical pin names
- [ ] Serial output shows GPIO##(Physical) format

---

## 📞 Troubleshooting by Pin

| Symptom | Check Pin | Solution |
|---------|-----------|----------|
| No WiFi LED | GPIO13 (D13) | Check LED polarity, pin connection |
| No data transmission | GPIO12 (D12) | Check LED polarity, pin connection |
| Buzzer not working | GPIO26 (D26) | Check if buzzer is active-HIGH |
| RFID not detected | GPIO19/21/23 | Check SPI bus connections |
| RFID not resetting | GPIO27 (D27) | Verify RST wire, check polarity |
| DHT not reading | GPIO25 (D25) | Add pull-up resistor (4.7kΩ), check data line |
| Sound always 0 | GPIO35 (VP) | Check analog input, verify power to sensor |
| CO always 0 | GPIO32 (SVP) | Check analog input, verify power to sensor |

---

## 📚 Reference Links

- **ESP32 DevKit Schematic**: Search "ESP32-DEVKITC" for official docs
- **RFID RC522 Datasheet**: Search "MFRC522" for specifications
- **DHT22 Datasheet**: Includes timing diagrams for data line
- **Arduino SPI Library**: https://www.arduino.cc/reference/en/#SPI

---

**Document Version:** 1.1  
**Last Updated:** February 28, 2026  
**For:** Classroom Management System ESP32 Firmware
