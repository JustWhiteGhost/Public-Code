#include <Wire.h>
#include <VL53L0X_mod.h>
#include <Adafruit_NeoPixel.h>

#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif


void IRAM_ATTR onSoundISR();

VL53L0X_mod sensor;


const int MIN_DISTANCE = 100;  
const int MAX_DISTANCE = 800;  
// LED Matrix Configuration
#define LED_PIN 16   
#define NUM_LEDS 64    
#define BRIGHTNESS 10  
#define BUTTON_PIN 6   

// Animation state
unsigned long lastAnimationUpdate = 0;
unsigned long animationDelay = 50;
float animationProgress = 0;

// Mode switching
int currentMode = 0;
const int NUM_MODES = 3; 
bool lastButtonState = HIGH;
unsigned long lastButtonPress = 0;
const unsigned long buttonDebounce = 200;

float expandingCircleSpeed = 0.15;

// Sound Sensor Configuration
#define SOUND_SENSOR_PIN 15 

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// System state
bool isSystemOn = false;   
unsigned long lastTriggerTime = 0;
unsigned long debounceDelay = 500; 

// Interrupt flags
volatile bool soundEvent = false;
volatile unsigned long soundEventMs = 0;

void onActivate() {
  Serial.println("System: ON\n");
}

void onDeactivate() {
  Serial.println("System: OFF\n");
  strip.clear();
  strip.show();
  resetgame();
}

void onActiveLoop() {
    // Run current mode
  switch(currentMode) {
    case 0:
      mode_Project();
      break;
    case 1:
      mode_Game();
      break;
    case 2:
      cycleAnimations();
      break;
  }
  
}

// --- Core logic ---
void toggleSystem() {
  isSystemOn = !isSystemOn;
  if (isSystemOn) {
    onActivate();
  } else {
    onDeactivate();
  }
}

// Handle any pending sound events (debounced in the main loop)
void checkSoundSensor() {
  bool hasEvent = false;
  unsigned long eventAt = 0;

  noInterrupts();
  if (soundEvent) {
    hasEvent = true;
    eventAt = soundEventMs;
    soundEvent = false;  // clear the flag
  }
  interrupts();

  if (!hasEvent) return;

  unsigned long now = millis();
  if (now - lastTriggerTime > debounceDelay) {
    lastTriggerTime = now;
    Serial.println("*** Sound detected! (IRQ) ***");
    toggleSystem();
  }
}

void IRAM_ATTR onSoundISR() {
  soundEvent = true;
  soundEventMs = millis();
}

void setup() {
  Serial.begin(9600);

  // Initialize distance sensor
  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();  
  sensor.init();
  sensor.setTimeout(500);

  // Initialize LED strip
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.clear();
  strip.show();

  // Initialize  Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize sound sensor
  pinMode(SOUND_SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SOUND_SENSOR_PIN), onSoundISR, FALLING); // KY-038 goes LOW on sound

  Serial.println("=== Sound-Activated System ===");
  Serial.println("System initialized");
  Serial.print("LED Matrix pin: ");
  Serial.println(LED_PIN);
  Serial.print("Sound sensor pin: ");
  Serial.println(SOUND_SENSOR_PIN);
  Serial.println("NOTE: KY-038 outputs LOW (0) when sound detected");
  Serial.println("System is OFF - Clap to turn ON\n");
}

void loop() {
  checkSoundSensor();
  checkButton();
  if (isSystemOn) {
    onActiveLoop();
  }
}