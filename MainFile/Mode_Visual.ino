// Animation cycling
unsigned long lastAnimationSwitch = 0;
int currentAnimation = 0;
const int NUM_ANIMATIONS = 6;

void cycleAnimations() {
  unsigned long now = millis();
  
  // Switch animation every 5 seconds
  if (now - lastAnimationSwitch > 5000) {
    lastAnimationSwitch = now;
    currentAnimation = (currentAnimation + 1) % NUM_ANIMATIONS;
    strip.clear();
    strip.show();
  }
  
  // Call the current animation
  switch(currentAnimation) {
    case 0:
      anim_smiley();
      break;
    case 1:
      anim_sad();
      break;
    case 2:
      anim_loading();
      break;
    case 3:
      anim_snake();
      break;
    case 4:
      anim_dvd();
      break;
    case 5:
      anim_stars();
      break;
  }
}