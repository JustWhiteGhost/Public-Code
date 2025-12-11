void tooClose(int distance) {
  drawSadFace();
}

void tooFar(int distance) {
  drawPulsingCircle(strip.Color(255, 50, 0));  // Orange pulsing
}

void inRange(int distance) {
  float proximity = 100.0 - ((distance - MIN_DISTANCE) * 100.0 / (MAX_DISTANCE - MIN_DISTANCE));

  expandingCircleSpeed = 0.05 + (proximity / 100.0) * 0.3;  // Range: 0.05 to 0.35
  
  drawExpandingCircle(strip.Color(0, 150, 255));  // Blue expanding circle
}

void mode_Project(){
  int distance = sensor.readRangeSingleMillimeters();
  
  if (sensor.timeoutOccurred()) {
    uint32_t red = strip.Color(255, 0, 0);
    drawBorder(red);
    drawAlertSign(red);
    strip.clear();
    strip.show();
    delay(200);
    return;
  }
  
  // Update animation frame timing
  unsigned long currentTime = millis();
  if (currentTime - lastAnimationUpdate >= animationDelay) {
    lastAnimationUpdate = currentTime;
    
    strip.clear();
    
    if (distance < MIN_DISTANCE) {
      tooClose(distance);
    } 
    else if (distance > MAX_DISTANCE) {
      tooFar(distance);
    } 
    else {
      inRange(distance);
    }
    
    strip.show();
  }
  
  delay(50);
}