# 🚀 Distance and Temperature Sensing Using Arduino Uno

A comprehensive Arduino Uno project demonstrating real-time distance and temperature measurement using external sensors with serial communication.

---

## 📌 Project Overview

This project showcases how to integrate multiple sensors with Arduino Uno to create a functional IoT device capable of measuring environmental conditions. The system reads data from temperature and ultrasonic distance sensors, processes the information, and displays results on the Serial Monitor for real-time monitoring.

**Key Capabilities:**
- ✅ Real-time temperature monitoring
- ✅ Accurate distance measurement

---

## 🎯 Features

| Feature | Description |
|---------|-------------|
| **Temperature Sensing** | Real-time temperature data collection with high accuracy |
| **Distance Measurement** | Ultrasonic sensor for non-contact distance detection |
| **Easy Integration** | Simple wiring and straightforward code |

---

## 🛠️ Hardware Requirements

### Components Needed:
- **Arduino Uno** (Microcontroller)
- **Temperature Sensor** :
  - Temperature Sensor [TMP36]
- **Ultrasonic Sensor** (HC-SR04)
  - Measurement Range: 2cm - 400cm
  - Accuracy: ±3mm
- **Breadboard** (400-pin or larger)
- **Jumper Wires** (Male to Male, Male to Female)
- **USB Cable** (Type A to Type B)
- **Power Supply** (5V for Arduino)

### Pin Configuration:

```
Arduino Pin | Sensor | Connection
------------|--------|------------
D2          | DHT    | Data Pin
D9          | HC-SR04| Trigger Pin
D10         | HC-SR04| Echo Pin
5V          | All    | Power
GND         | All    | Ground
```

---

## 🔌 Wiring Diagram

### HC-SR04 Ultrasonic Sensor:
```
HC-SR04       Arduino Uno
VCC      ---- 5V
GND      ---- GND
TRIG     ---- D9
ECHO     ---- D10
```

### DHT11/DHT22 Temperature Sensor:
```
DHT        Arduino Uno
VCC   ---- 5V
GND   ---- GND
DATA  ---- D2
```

### LM35 Temperature Sensor:
```
LM35          Arduino Uno
VCC      ---- 5V
GND      ---- GND
VOUT     ---- A0
```

---

## 💻 Software Setup

### Step 1: Install Arduino IDE
- Download from [arduino.cc](https://www.arduino.cc/en/software)
- Install on your computer

### Step 2: Install Required Libraries
```
Sketch → Include Library → Manage Libraries

Search for:
- DHT sensor library (if using DHT)
- Install Adafruit DHT library
```

### Step 3: Connect Hardware
- Assemble circuit according to wiring diagram
- Connect Arduino to PC via USB cable

### Step 4: Upload Code
```
1. Open Arduino IDE
2. Paste your code
3. Select Board: Tools → Board → Arduino Uno
4. Select Port: Tools → Port → COM (your port)
5. Click Upload
```

### Step 5: Monitor Data
```
Tools → Serial Monitor
Set Baud Rate: 9600
Watch real-time sensor data
```

---

## 📊 Output Format

The Serial Monitor displays data in this format:

```
Temperature: 25.5 °C
Distance: 15.3 cm
-----------
Temperature: 25.6 °C
Distance: 15.2 cm
```

---

## 💡 Sample Arduino Code

```cpp
// Temperature & Distance Sensing

// DHT Sensor
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic Sensor
#define TRIGGER_PIN 9
#define ECHO_PIN 10

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Read Temperature
  float temp = dht.readTemperature();
  
  // Read Distance
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  
  // Print Data
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  Serial.println("-----------");
  delay(1000);
}
```

---

## 🔧 Troubleshooting

| Problem | Solution |
|---------|----------|
| **No Serial Output** | Check USB connection, verify COM port, reload code |
| **Temperature reads 0** | Verify sensor connections, install DHT library |
| **Distance reads 0** | Check HC-SR04 trigger/echo pins, verify 5V power |
| **Inconsistent Readings** | Add delay between measurements, check sensor placement |
| **Arduino not recognized** | Install CH340 drivers (if needed), try different USB port |

---

## 📈 Applications

- 🏠 Smart Home Automation
- 🌡️ Environmental Monitoring Systems
- 🤖 Robotics and Obstacle Detection
- 📍 Distance Measurement Tools

---

## 🤝 Contributing

Contributions are welcome! Feel free to:
- 🐛 Report bugs
- 💡 Suggest improvements
- 🔄 Submit pull requests
- 📝 Improve documentation

---

## 📄 License

This project is licensed under the **MIT License** - see the LICENSE file for details.

---

## 📞 Support & Questions

- **Documentation:** Check the comments in the code
- **Issues:** Open an GitHub issue
- **Questions:** Start a discussion

---

## ⭐ Acknowledgments

- Arduino Community
- Sensor Manufacturers
- Open Source Contributors

---

**Last Updated:** September 2026  
**Status:** ✅ Active & Maintained

---

### Quick Links
- [Arduino Official Website](https://www.arduino.cc/)
