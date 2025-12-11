/*
 * Multi-Module Arduino Control System
 * Modules: Soil Sensor, DHT11, SG90 Servo, Speaker, LCD Display
 * 
 * Pin Configuration:
 * - Soil Sensor Analog: A0
 * - Soil Sensor Digital: 2
 * - DHT11 Data: 4
 * - Servo Signal: 9
 * - Speaker: 8
 * - LCD: RS=12, E=11, D4=7, D5=6, D6=5, D7=3
 */

#include <DHT.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// Pin Definitions
#define SOIL_ANALOG_PIN A0
#define SOIL_DIGITAL_PIN 2
#define DHT_PIN 4
#define SERVO_PIN 9
#define SPEAKER_PIN 8

// LCD Pin Definitions (4-bit mode)
#define LCD_RS 12
#define LCD_E 11
#define LCD_D4 7
#define LCD_D5 6
#define LCD_D6 5
#define LCD_D7 3

// LCD Setup (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// DHT11 Setup
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// Servo Setup
Servo myServo;

// Global Variables
int soilMoistureValue = 0;
int soilMoisturePercent = 0;
bool isSoilDry = false;

// ============================================
// SETUP FUNCTION
// ============================================
void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("Multi-Module System Initialized");
  Serial.println("=================================");
  
  // Setup Soil Sensor
  pinMode(SOIL_ANALOG_PIN, INPUT);
  pinMode(SOIL_DIGITAL_PIN, INPUT);
  Serial.println("✓ Soil Sensor Ready");
  
  // Setup DHT11
  dht.begin();
  Serial.println("✓ DHT11 Sensor Ready");
  
  // Setup Servo
  myServo.attach(SERVO_PIN);
  myServo.write(90); // Initialize to center position
  Serial.println("✓ Servo Motor Ready");
  
  // Setup Speaker
  pinMode(SPEAKER_PIN, OUTPUT);
  Serial.println("✓ Speaker Ready");
  
  // Setup LCD Display
  lcd.begin(16, 2);  // 16 columns, 2 rows
  lcd.clear();
  lcdWrite("System Ready!");
  Serial.println("✓ LCD Display Ready");
  
  Serial.println("=================================\n");
  
  // Startup Sound
  playStartupTone();
  delay(1000);
  lcd.clear();
}

// ============================================
// MAIN LOOP
// ============================================
void loop() {
  // Read all sensors
  readSoilHumidity();
  readDHT11();
  
  // Display readings
  displayReadings();
  
  // Update LCD with sensor data
  lcdDisplaySensorData();
  
  // Example automated actions based on readings
  automatedControl();
  
  delay(2000); // Update every 2 seconds
}

// ============================================
// SOIL SENSOR FUNCTIONS
// ============================================
void readSoilHumidity() {
  // Read analog value (0-1023)
  soilMoistureValue = analogRead(SOIL_ANALOG_PIN);
  
  // Convert to percentage (0-100%)
  // Note: Lower analog value = more moisture
  soilMoisturePercent = map(soilMoistureValue, 1023, 0, 0, 100);
  
  // Read digital output
  isSoilDry = digitalRead(SOIL_DIGITAL_PIN);
}

int getSoilHumidity() {
  return soilMoisturePercent;
}

int getSoilRawValue() {
  return soilMoistureValue;
}

bool isSoilMoistureLow() {
  return isSoilDry;
}

// ============================================
// DHT11 SENSOR FUNCTIONS
// ============================================
float getTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Error: Failed to read temperature!");
    return -999;
  }
  return temp;
}

float getHumidity() {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Error: Failed to read humidity!");
    return -999;
  }
  return humidity;
}

void readDHT11() {
  // Readings are updated in memory by DHT library
  delay(10);
}

// ============================================
// SERVO MOTOR FUNCTIONS
// ============================================
void rotateServo(int degrees) {
  // Constrain angle to 0-180 degrees
  degrees = constrain(degrees, 0, 180);
  myServo.write(degrees);
  Serial.print("→ Servo rotated to: ");
  Serial.print(degrees);
  Serial.println("°");
}

void sweepServo() {
  Serial.println("→ Servo sweeping...");
  for (int pos = 0; pos <= 180; pos += 5) {
    myServo.write(pos);
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos -= 5) {
    myServo.write(pos);
    delay(15);
  }
}

// ============================================
// SPEAKER FUNCTIONS
// ============================================
void playTone(int frequency, int duration) {
  tone(SPEAKER_PIN, frequency, duration);
  delay(duration);
  noTone(SPEAKER_PIN);
}

void playAlert() {
  playTone(1000, 200);
  delay(100);
  playTone(1000, 200);
  delay(100);
  playTone(1000, 200);
}

void playStartupTone() {
  playTone(523, 200);  // C
  playTone(659, 200);  // E
  playTone(784, 300);  // G
}

void playWarningBeep() {
  playTone(800, 500);
}

void playSuccessBeep() {
  playTone(1047, 150); // C
  playTone(1319, 150); // E
  playTone(1568, 300); // G
}

// ============================================
// LCD DISPLAY FUNCTIONS
// ============================================
void lcdWrite(String text) {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  // If text is longer than 16 characters, wrap to second line
  if (text.length() <= 16) {
    lcd.print(text);
  } else {
    lcd.print(text.substring(0, 16));
    lcd.setCursor(0, 1);
    lcd.print(text.substring(16, min((int)text.length(), 32)));
  }
}

void lcdWrite(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1.substring(0, min((int)line1.length(), 16)));
  lcd.setCursor(0, 1);
  lcd.print(line2.substring(0, min((int)line2.length(), 16)));
}

void lcdWriteAt(int col, int row, String text) {
  lcd.setCursor(col, row);
  lcd.print(text);
}

void lcdClear() {
  lcd.clear();
}

void lcdDisplaySensorData() {
  // Display soil moisture on line 1
  String line1 = "Soil: " + String(soilMoisturePercent) + "%";
  
  // Display temperature and humidity on line 2
  float temp = getTemperature();
  float hum = getHumidity();
  String line2 = String((int)temp) + "C " + String((int)hum) + "%RH";
  
  lcdWrite(line1, line2);
}

// ============================================
// DISPLAY FUNCTIONS
// ============================================
void displayReadings() {
  Serial.println("\n--- Sensor Readings ---");
  
  // Soil Moisture
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.print("% (Raw: ");
  Serial.print(soilMoistureValue);
  Serial.print(") - ");
  if (isSoilDry) {
    Serial.println("DRY ⚠");
  } else {
    Serial.println("WET ✓");
  }
  
  // Temperature
  float temp = getTemperature();
  if (temp != -999) {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println("°C");
  }
  
  // Humidity
  float humidity = getHumidity();
  if (humidity != -999) {
    Serial.print("Air Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }
  
  Serial.println("----------------------\n");
}

// ============================================
// AUTOMATED CONTROL EXAMPLE
// ============================================
void automatedControl() {
  // Example: If soil is too dry, alert and rotate servo
  if (soilMoisturePercent < 30) {
    Serial.println("⚠ ALERT: Soil moisture is LOW!");
    lcdWrite("ALERT!", "Soil too dry!");
    playWarningBeep();
    rotateServo(45); // Rotate to watering position
    delay(1000);
    rotateServo(90); // Return to center
    delay(500);
  }
  
  // Example: If temperature is high, alert
  float temp = getTemperature();
  if (temp > 30 && temp != -999) {
    Serial.println("⚠ ALERT: High temperature detected!");
    lcdWrite("ALERT!", "Temp High: " + String((int)temp) + "C");
    playAlert();
    delay(1000);
  }
}

// ============================================
// UTILITY FUNCTIONS
// ============================================
void printSystemStatus() {
  Serial.println("\n=== SYSTEM STATUS ===");
  Serial.println("All modules operational");
  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");
  Serial.println("====================\n");
}

// Custom function to get comprehensive soil analysis
String getSoilCondition() {
  if (soilMoisturePercent > 70) return "Very Wet";
  else if (soilMoisturePercent > 50) return "Moist";
  else if (soilMoisturePercent > 30) return "Adequate";
  else if (soilMoisturePercent > 15) return "Dry";
  else return "Very Dry";
}
