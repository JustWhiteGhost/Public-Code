int targetDistance = 400;
unsigned long gameStartTime = 0;
int score = 0;
bool gameActive = false;
unsigned long lastGameUpdate = 0;

void resetGameMode() {
  targetDistance = random(200, 700);
  gameStartTime = millis();
  gameActive = true;
  Serial.print("New target: ");
  Serial.print(targetDistance);
  Serial.println(" mm");
}

void resetgame(){
  score = 0;
}

void mode_Game() {
  int distance = sensor.readRangeSingleMillimeters();
  unsigned long now = millis();
  
  // Update at reasonable rate
  if (now - lastGameUpdate < 50) {
    return;
  }
  lastGameUpdate = now;
  
  strip.clear();
  
  if (!gameActive) {
    // Show score as filled LEDs
    int ledsToLight = min(score, NUM_LEDS);
    for (int i = 0; i < ledsToLight; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show(); // *** ADDED THIS ***
    
    // Start new round after 2 seconds
    if (now - gameStartTime > 2000) {
      resetGameMode();
    }
    return;
  }
  
  // Calculate how close to target
  int diff = abs(distance - targetDistance);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" mm, Diff: ");
  Serial.println(diff);
  
  if (diff < 20) {
    // Hit! Flash green
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    score++;
    gameActive = false;
    gameStartTime = now;
    Serial.print("Hit! Score: ");
    Serial.println(score);
    
  } else if (diff < 50) {
    // Very close - yellow
    drawTargetIndicator(strip.Color(255, 200, 0));
  } else if (diff < 100) {
    // Close - orange
    drawTargetIndicator(strip.Color(255, 100, 0));
  } else {
    // Far - red, show direction
    if (distance < targetDistance) {
      drawArrowUp(strip.Color(255, 0, 0));
    } else {
      drawArrowDown(strip.Color(255, 0, 0));
    }
  }
  
  strip.show(); // *** ADDED THIS ***
}