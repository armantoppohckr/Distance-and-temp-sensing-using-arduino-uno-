// ============================================================================
// Distance and Temperature Sensing with Alert System
// ============================================================================
// Project: Advanced sensor monitoring with LED indicators and buzzer alerts
// Author: Your Name
// Date: September 2026
// Description: Monitors distance and temperature with visual/audio alerts
//              and silent mode toggle
// ============================================================================

// ============================================================================
// PIN DEFINITIONS
// ============================================================================
const int TRIG_PIN    = 9;      // Ultrasonic trigger pin
const int ECHO_PIN    = 8;      // Ultrasonic echo pin
const int TEMP_PIN    = A1;     // Temperature sensor analog pin
const int BUTTON_PIN  = 2;      // Mode toggle button pin
const int BUZZER_PIN  = 10;     // Buzzer/speaker pin
const int LED_GREEN   = 5;      // Safe status LED
const int LED_YELLOW  = 6;      // Warning status LED
const int LED_RED     = 7;      // Danger status LED

// ============================================================================
// THRESHOLD VALUES
// ============================================================================
const int DIST_SAFE    = 50;    // Safe distance (cm)
const int DIST_WARN    = 20;    // Warning distance (cm)
const float TEMP_ALERT = 40.0;  // Temperature alert threshold (°C)

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================
bool mode_silent = false;          // Silent mode flag
unsigned long Time = 0;            // Buzzer timing variable
bool buzzerOn = false;             // Buzzer state tracker

// ============================================================================
// SETUP - Runs once when Arduino starts
// ============================================================================
void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  
  // Configure Ultrasonic Sensor Pins
  pinMode(TRIG_PIN, OUTPUT);       // Trigger as output
  pinMode(ECHO_PIN, INPUT);        // Echo as input
  
  // Configure Input Button
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button with internal pullup
  
  // Configure Output Devices
  pinMode(BUZZER_PIN, OUTPUT);     // Buzzer pin
  pinMode(LED_GREEN, OUTPUT);      // Green LED
  pinMode(LED_YELLOW, OUTPUT);     // Yellow LED
  pinMode(LED_RED, OUTPUT);        // Red LED
  
  Serial.println("====================================");
  Serial.println("Advanced Sensor Monitoring System");
  Serial.println("System Initialized!");
  Serial.println("====================================");
  delay(1000);
}

// ============================================================================
// MAIN LOOP - Runs repeatedly
// ============================================================================
void loop() {
  
  // ========== STEP 1: Read Distance from Ultrasonic Sensor ==========
  float distance = measureDistance();
  
  // ========== STEP 2: Read Temperature from Analog Sensor ==========
  float realtemp = measureTemperature();
  
  // ========== STEP 3: Determine Status Based on Distance ==========
  bool objectTooClose = false;
  
  if (distance > DIST_SAFE) {
    // ===== SAFE STATUS (Green) =====
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER_PIN);
    buzzerOn = false;
    
  } else if (distance <= DIST_SAFE && distance > DIST_WARN) {
    // ===== WARNING STATUS (Yellow) =====
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER_PIN);
    buzzerOn = false;
    
  } else {
    // ===== DANGER STATUS (Red) =====
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    objectTooClose = true;
    tone(BUZZER_PIN, 1200);  // 1200 Hz buzzer
    buzzerOn = true;
  }
  
  // ========== STEP 4: Check Temperature Alert ==========
  if (realtemp >= TEMP_ALERT) {
    digitalWrite(LED_RED, HIGH);  // Enable red LED for high temp
  }
  
  // ========== STEP 5: Handle Button Press for Silent Mode ==========
  handleButtonPress();
  
  // ========== STEP 6: Apply Silent Mode ==========
  if (mode_silent) {
    noTone(BUZZER_PIN);
    buzzerOn = false;
  }
  
  // ========== STEP 7: Buzzer Beeping in Alert Mode ==========
  if (objectTooClose && !mode_silent) {
    if (millis() - Time >= 200) {  // 200ms beep interval
      if (!buzzerOn) {
        tone(BUZZER_PIN, 1200);
        buzzerOn = true;
      } else {
        noTone(BUZZER_PIN);
        buzzerOn = false;
      }
      Time = millis();
    }
  }
  
  // ========== STEP 8: Log Data to Serial Monitor ==========
  logSensorData(distance, realtemp, objectTooClose);
}

// ============================================================================
// FUNCTION: Measure Distance from Ultrasonic Sensor
// ============================================================================
float measureDistance() {
  // Send trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(TRIG_PIN, LOW);
  
  // Measure echo time
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance
  // Formula: Distance = (Speed × Time) / 2
  // Speed of sound = 343 m/s = 0.0343 cm/µs
  float distance = duration * 0.0343 / 2.0;
  
  return distance;
}

// ============================================================================
// FUNCTION: Measure Temperature from Analog Sensor
// ============================================================================
float measureTemperature() {
  // Read analog value (0-1023)
  int temp = analogRead(TEMP_PIN);
  
  // Convert to voltage (0-5V)
  float voltage = temp * 5.0 / 1023.0;
  
  // Convert voltage to temperature (LM35: 10mV per °C)
  // Offset 0.5V at 0°C
  float realtemp = (voltage - 0.5) * 100.0;
  
  return realtemp;
}

// ============================================================================
// FUNCTION: Handle Button Press for Mode Toggle
// ============================================================================
void handleButtonPress() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  // Detect button press (HIGH to LOW transition)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    mode_silent = !mode_silent;  // Toggle silent mode
    delay(50);  // Debounce delay
    
    // Provide feedback
    if (mode_silent) {
      Serial.println(">> SILENT MODE ENABLED");
    } else {
      Serial.println(">> MONITORING MODE ENABLED");
    }
  }
  
  lastButtonState = currentButtonState;
}

// ============================================================================
// FUNCTION: Log Sensor Data to Serial Monitor
// ============================================================================
void logSensorData(float distance, float realtemp, bool objectTooClose) {
  static unsigned long lastLogTime = 0;
  
  // Log every 1000ms (1 second)
  if (millis() - lastLogTime >= 1000) {
    
    Serial.println("----------------------------");
    
    // Display current mode
    Serial.print("Mode: ");
    Serial.println(mode_silent ? "SILENT" : "MONITORING");
    
    // Display distance
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    // Display temperature
    Serial.print("Temperature: ");
    Serial.print(realtemp);
    Serial.println(" °C");
    
    // Display warnings
    if (objectTooClose) {
      Serial.println("⚠️  WARNING: Object too close!");
    }
    
    if (realtemp >= TEMP_ALERT) {
      Serial.println("🔴 ALERT: High Temperature Detected!");
    }
    
    Serial.println("----------------------------");
    Serial.println("");
    
    lastLogTime = millis();
  }
}

// ============================================================================
// CONFIGURATION GUIDE
// ============================================================================
/*
 * PINOUT:
 * Pin 2 (D2)   → Button (with pullup)
 * Pin 5 (D5)   → Green LED (Safe)
 * Pin 6 (D6)   → Yellow LED (Warning)
 * Pin 7 (D7)   → Red LED (Danger)
 * Pin 8 (D8)   → Ultrasonic Echo
 * Pin 9 (D9)   → Ultrasonic Trigger
 * Pin 10 (D10) → Buzzer
 * Pin A1 (A1)  → Temperature Sensor (LM35)
 * 5V           → Power for all components
 * GND          → Ground for all components
 * 
 * THRESHOLDS (CUSTOMIZABLE):
 * - DIST_SAFE = 50cm  (Green LED)
 * - DIST_WARN = 20cm  (Yellow LED)
 * - TEMP_ALERT = 40°C (Red LED trigger)
 * 
 * SERIAL OUTPUT:
 * - Baud Rate: 9600
 * - Updates every 1 second
 * - Shows mode, distance, temperature, alerts
 * 
 * FEATURES:
 * ✓ Real-time distance monitoring (HC-SR04)
 * ✓ Temperature monitoring (LM35)
 * ✓ Multi-color LED status indicators
 * ✓ Audio alerts with buzzer
 * ✓ Silent mode toggle button
 * ✓ Serial data logging
 * ✓ Customizable thresholds
 */

// ============================================================================
// END OF CODE
// ============================================================================
