// Convert x,y coordinates to LED index (zigzag pattern for 8x8 matrix)
int xy(int x, int y) {
  if (x < 0 || x > 7 || y < 0 || y > 7) return -1;
  if (y % 2 == 0) {
    return y * 8 + x;
  } else {
    return y * 8 + (7 - x);
  }
}

// Calculate distance from center
float distanceFromCenter(int x, int y) {
  float centerX = 3.5;
  float centerY = 3.5;
  float dx = x - centerX;
  float dy = y - centerY;
  return sqrt(dx * dx + dy * dy);
}

// Draw expanding circle frame with adjustable speed
void drawExpandingCircle(uint32_t color) {
  float radius = animationProgress;
  
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      float dist = distanceFromCenter(x, y);
      
      if (dist <= radius && dist > radius - 0.8) {
        int ledIndex = xy(x, y);
        if (ledIndex >= 0) {
          strip.setPixelColor(ledIndex, color);
        }
      }
    }
  }
  
  // Update progress with speed modifier
  animationProgress += expandingCircleSpeed;
  if (animationProgress > 6.0) animationProgress = 0;
}

// Draw sad face (static)
void drawSadFace() {
  uint32_t blue = strip.Color(255, 50, 0);
  
  // Left eye (2x2)
  strip.setPixelColor(xy(1, 2), blue);
  strip.setPixelColor(xy(2, 2), blue);
  strip.setPixelColor(xy(1, 3), blue);
  strip.setPixelColor(xy(2, 3), blue);
  
  // Right eye (2x2)
  strip.setPixelColor(xy(5, 2), blue);
  strip.setPixelColor(xy(6, 2), blue);
  strip.setPixelColor(xy(5, 3), blue);
  strip.setPixelColor(xy(6, 3), blue);
  
  // Frown (curved down)
  strip.setPixelColor(xy(1, 6), blue);
  strip.setPixelColor(xy(2, 5), blue);
  strip.setPixelColor(xy(3, 5), blue);
  strip.setPixelColor(xy(4, 5), blue);
  strip.setPixelColor(xy(5, 5), blue);
  strip.setPixelColor(xy(6, 6), blue);
}

// Draw pulsing circle frame
void drawPulsingCircle(uint32_t color) {
  float maxDist = sqrt(3.5 * 3.5 + 3.5 * 3.5);
  float radius;
  
  // Create pulsing effect
  if (animationProgress < 5.0) {
    radius = animationProgress;  // Expand
  } else {
    radius = 10.0 - animationProgress;  // Contract
  }
  
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      float dist = distanceFromCenter(x, y);
      
      if (dist <= radius) {
        int ledIndex = xy(x, y);
        if (ledIndex >= 0) {
          float brightness = 1.0 - (dist / maxDist) * 0.5;
          uint8_t r = ((color >> 16) & 0xFF) * brightness;
          uint8_t g = ((color >> 8) & 0xFF) * brightness;
          uint8_t b = (color & 0xFF) * brightness;
          strip.setPixelColor(ledIndex, strip.Color(r, g, b));
        }
      }
    }
  }
  
  // Update progress for pulsing effect
  animationProgress += 0.5;
  if (animationProgress > 10.0) animationProgress = 0;
}

// Check button for mode switching
void checkButton() {
  bool buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == LOW && lastButtonState == HIGH) {
    unsigned long now = millis();
    if (now - lastButtonPress > buttonDebounce) {
      Serial.print("Switching");
      lastButtonPress = now;
      currentMode = (currentMode + 1) % NUM_MODES;
      
      Serial.print("Mode switched to: ");
      Serial.println(currentMode);
    }
  }
  
  lastButtonState = buttonState;
}


void drawTargetIndicator(uint32_t color) {
  // Pulsing circle
  float pulse = (sin(millis() / 100.0) + 1.0) / 2.0;
  float radius = 2.0 + pulse * 2.0;
  
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      float dx = x - 3.5;
      float dy = y - 3.5;
      float d = sqrt(dx * dx + dy * dy);
      
      if (d <= radius && d > radius - 0.8) {
        int i = xy(x, y);
        if (i >= 0) strip.setPixelColor(i, color);
      }
    }
  }
}

void drawArrowUp(uint32_t color) {
  // Arrow pointing up
  strip.setPixelColor(xy(3, 1), color);
  strip.setPixelColor(xy(4, 1), color);
  strip.setPixelColor(xy(2, 2), color);
  strip.setPixelColor(xy(5, 2), color);
  strip.setPixelColor(xy(3, 3), color);
  strip.setPixelColor(xy(4, 3), color);
  strip.setPixelColor(xy(3, 4), color);
  strip.setPixelColor(xy(4, 4), color);
  strip.setPixelColor(xy(3, 5), color);
  strip.setPixelColor(xy(4, 5), color);
}

void drawArrowDown(uint32_t color) {
  // Arrow pointing down
  strip.setPixelColor(xy(3, 2), color);
  strip.setPixelColor(xy(4, 2), color);
  strip.setPixelColor(xy(3, 3), color);
  strip.setPixelColor(xy(4, 3), color);
  strip.setPixelColor(xy(3, 4), color);
  strip.setPixelColor(xy(4, 4), color);
  strip.setPixelColor(xy(2, 5), color);
  strip.setPixelColor(xy(5, 5), color);
  strip.setPixelColor(xy(3, 6), color);
  strip.setPixelColor(xy(4, 6), color);
}

// ========== ANIMATION 1: SMILEY FACE ==========
void anim_smiley() {
  strip.clear();
  uint32_t c = strip.Color(255, 200, 0);  // Yellow
  
  // Eyes (2x2 each)
  strip.setPixelColor(xy(1, 2), c);
  strip.setPixelColor(xy(2, 2), c);
  strip.setPixelColor(xy(1, 3), c);
  strip.setPixelColor(xy(2, 3), c);
  
  strip.setPixelColor(xy(5, 2), c);
  strip.setPixelColor(xy(6, 2), c);
  strip.setPixelColor(xy(5, 3), c);
  strip.setPixelColor(xy(6, 3), c);
  
  // Smile
  strip.setPixelColor(xy(1, 6), c);
  strip.setPixelColor(xy(6, 6), c);
  for (int i = 2; i <= 5; i++) {
    strip.setPixelColor(xy(i, 5), c);
  }
  
  strip.show(); // *** ADDED ***
}

// ========== ANIMATION 2: SAD FACE ==========
void anim_sad() {
  strip.clear();
  uint32_t c = strip.Color(100, 150, 255);  // Blue
  
  // Eyes (2x2 each)
  strip.setPixelColor(xy(1, 2), c);
  strip.setPixelColor(xy(2, 2), c);
  strip.setPixelColor(xy(1, 3), c);
  strip.setPixelColor(xy(2, 3), c);
  
  strip.setPixelColor(xy(5, 2), c);
  strip.setPixelColor(xy(6, 2), c);
  strip.setPixelColor(xy(5, 3), c);
  strip.setPixelColor(xy(6, 3), c);
  
  // Frown (inverted smile)
  strip.setPixelColor(xy(1, 5), c);
  strip.setPixelColor(xy(6, 5), c);
  for (int i = 2; i <= 5; i++) {
    strip.setPixelColor(xy(i, 6), c);
  }
  
  strip.show(); // *** ADDED ***
}

// ========== ANIMATION 3: LOADING CIRCLE ==========
float spinAngle = 0;
unsigned long lastLoadingUpdate = 0;

void anim_loading() {
  unsigned long now = millis();
  
  // Update rotation every 30ms
  if (now - lastLoadingUpdate > 30) {
    lastLoadingUpdate = now;
    spinAngle += 0.2; // Rotation speed
    if (spinAngle > TWO_PI) spinAngle -= TWO_PI;
  }
  
  strip.clear();
  
  uint32_t color = strip.Color(0, 200, 255);
  
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      float dx = x - 3.5;
      float dy = y - 3.5;
      float d = sqrt(dx * dx + dy * dy);
      
      float angle = atan2(dy, dx);
      if (angle < 0) angle += TWO_PI;
      
      float angleDiff = angle - spinAngle;
      if (angleDiff < 0) angleDiff += TWO_PI;
      
      if (d >= 2.0 && d <= 3.5 && angleDiff < (TWO_PI * 0.75)) {
        int i = xy(x, y);
        if (i >= 0) strip.setPixelColor(i, color);
      }
    }
  }
  
  strip.show();
}
// ========== ANIMATION 4: AUTO SNAKE ==========
struct SnakeSegment {
  int x, y;
};

SnakeSegment snake[64];
int snakeLength = 4;
int snakeDirX = 1;
int snakeDirY = 0;
unsigned long lastSnakeMove = 0;
int snakeMoveDelay = 200;

void resetSnake() {
  snakeLength = 4;
  snakeDirX = 1;
  snakeDirY = 0;
  for (int i = 0; i < snakeLength; i++) {
    snake[i].x = 3 - i;
    snake[i].y = 3;
  }
}

void anim_snake() {
  unsigned long now = millis();
  
  if (now - lastSnakeMove > snakeMoveDelay) {
    lastSnakeMove = now;
    
    // Calculate new head position
    int newX = snake[0].x + snakeDirX;
    int newY = snake[0].y + snakeDirY;
    
    // Wrap around edges
    if (newX < 0) newX = 7;
    if (newX > 7) newX = 0;
    if (newY < 0) newY = 7;
    if (newY > 7) newY = 0;
    
    // Random direction change (20% chance)
    if (random(100) < 20) {
      int dir = random(4);
      switch(dir) {
        case 0: snakeDirX = 1; snakeDirY = 0; break;  // Right
        case 1: snakeDirX = -1; snakeDirY = 0; break; // Left
        case 2: snakeDirX = 0; snakeDirY = 1; break;  // Down
        case 3: snakeDirX = 0; snakeDirY = -1; break; // Up
      }
    }
    
    // Move snake body
    for (int i = snakeLength - 1; i > 0; i--) {
      snake[i] = snake[i - 1];
    }
    snake[0].x = newX;
    snake[0].y = newY;
  }
  
  // Draw snake
  strip.clear();
  for (int i = 0; i < snakeLength; i++) {
    int brightness = 255 - (i * 30);
    if (brightness < 50) brightness = 50;
    strip.setPixelColor(xy(snake[i].x, snake[i].y), strip.Color(0, brightness, 0));
  }
  
  strip.show(); // *** ADDED ***
}

// ========== ANIMATION 5: DVD BOUNCING BOX ==========
float dvdX = 3.0;
float dvdY = 3.0;
float dvdVelX = 0.15;
float dvdVelY = 0.1;
uint32_t dvdColor = strip.Color(255, 0, 255);
unsigned long lastDvdUpdate = 0;

void anim_dvd() {
  unsigned long now = millis();
  
  if (now - lastDvdUpdate > 30) {
    lastDvdUpdate = now;
    
    // Update position
    dvdX += dvdVelX;
    dvdY += dvdVelY;
    
    // Bounce off walls (2x1 box)
    if (dvdX <= 0 || dvdX >= 6) {
      dvdVelX = -dvdVelX;
      dvdColor = strip.Color(random(256), random(256), random(256));
    }
    if (dvdY <= 0 || dvdY >= 7) {
      dvdVelY = -dvdVelY;
      dvdColor = strip.Color(random(256), random(256), random(256));
    }
    
    // Constrain
    dvdX = constrain(dvdX, 0, 6);
    dvdY = constrain(dvdY, 0, 7);
  }
  
  // Draw 2x1 box
  strip.clear();
  int x = (int)dvdX;
  int y = (int)dvdY;
  strip.setPixelColor(xy(x, y), dvdColor);
  strip.setPixelColor(xy(x + 1, y), dvdColor);
  
  strip.show(); // *** ADDED ***
}

// ========== ANIMATION 6: TWINKLING STARS ==========
struct Star {
  int x, y;
  uint32_t color;
  float brightness;
  float fadeSpeed;
  bool active;
};

Star stars[6];
unsigned long lastStarUpdate = 0;

void initStars() {
  for (int i = 0; i < 6; i++) {
    stars[i].active = false;
  }
}

void anim_stars() {
  unsigned long now = millis();
  
  if (now - lastStarUpdate > 50) {
    lastStarUpdate = now;
    
    // Randomly activate stars
    if (random(100) < 15) {
      for (int i = 0; i < 6; i++) {
        if (!stars[i].active) {
          stars[i].x = random(8);
          stars[i].y = random(8);
          stars[i].color = strip.Color(random(256), random(256), random(256));
          stars[i].brightness = 1.0;
          stars[i].fadeSpeed = 0.02 + random(30) / 1000.0;
          stars[i].active = true;
          break;
        }
      }
    }
    
    // Update and draw stars
    strip.clear();
    for (int i = 0; i < 6; i++) {
      if (stars[i].active) {
        stars[i].brightness -= stars[i].fadeSpeed;
        
        if (stars[i].brightness <= 0) {
          stars[i].active = false;
        } else {
          uint8_t r = ((stars[i].color >> 16) & 0xFF) * stars[i].brightness;
          uint8_t g = ((stars[i].color >> 8) & 0xFF) * stars[i].brightness;
          uint8_t b = (stars[i].color & 0xFF) * stars[i].brightness;
          strip.setPixelColor(xy(stars[i].x, stars[i].y), strip.Color(r, g, b));
        }
      }
    }
  }
  
  strip.show(); // *** ADDED ***
}

void drawAlertSign(uint32_t color) {
  // Draw exclamation mark alert sign
  // Top part of exclamation
  strip.setPixelColor(xy(3, 1), color);
  strip.setPixelColor(xy(4, 1), color);
  strip.setPixelColor(xy(3, 2), color);
  strip.setPixelColor(xy(4, 2), color);
  strip.setPixelColor(xy(3, 3), color);
  strip.setPixelColor(xy(4, 3), color);
  strip.setPixelColor(xy(3, 4), color);
  strip.setPixelColor(xy(4, 4), color);
  
  // Bottom dot of exclamation
  strip.setPixelColor(xy(3, 6), color);
  strip.setPixelColor(xy(4, 6), color);

  strip.show();
}

void drawBorder(uint32_t color) {
  // Draw border around the grid
  for (int x = 0; x < 8; x++) {
    strip.setPixelColor(xy(x, 0), color);  // Top row
    strip.setPixelColor(xy(x, 7), color);  // Bottom row
  }
  for (int y = 1; y < 7; y++) {
    strip.setPixelColor(xy(0, y), color);  // Left column
    strip.setPixelColor(xy(7, y), color);  // Right column
  }

  strip.show();
}