#include <DigiMouse.h>

////////////////////////////////////////////////////////////
// I/O pin assigns
//#define OUTPUT_LED 0 // LED assign for Model B
#define OUTPUT_LED 1 // LED assign for Model A
#define STEP_MODE 4 // Mode for mouse move
#define STEP_LENGTH 5 // Length for mouse move
#define STEP_INTERVAL 300 // Interval for mouse move or maximum inteval when WAIT_RANDOM is not 0
#define STARTUP_INTERVAL 10 // Interval for confirming behavior of this mouse after plugging
#define STARTUP_COUNT 6 // Count for confirming behavior of this mouse after plugging
#define STEP_BACK 1 // Come back to start point after step
#define WAIT_RANDOM 1 // Wait step at random 

////////////////////////////////////////////////////////////
// Variables
int count = 0;
bool led = false;
const int heartbeat = 1; // sec
const int backInterval = 500;
int stepCount = 0;
const int steps = 4;
const int maxCount = 86400; // seconds for 1 day
int waiting = 0;
int waitFor = 0;
const float pi = atan(1) * 4;

////////////////////////////////////////////////////////////
// LED switch
void ledOn() {
  if (!led) {
    digitalWrite(OUTPUT_LED, HIGH);
  }
  led = true;
}
void ledOff() {
  if (led) {
    digitalWrite(OUTPUT_LED, LOW);
  }
  led = false;
}
void ledBlink() {
  if (count % 2) {
    ledOn();
  } else {
    ledOff();
  }
}

////////////////////////////////////////////////////////////
// Mouse moves
void stepTo(int x, int y) {
  DigiMouse.moveX(x);
  DigiMouse.moveY(y);
  stepBack(x, y);
}
void stepBack(int x, int y) {
  DigiMouse.delay(backInterval);
  if (STEP_BACK) {
    DigiMouse.moveX(x * -1);
    DigiMouse.moveY(y * -1);
  }
}
void boxStep() {
  stepCount++;
  if (stepCount % steps == 0) {
    stepTo(STEP_LENGTH, 0);
  } else if (stepCount % steps == 1) {
    stepTo(0, STEP_LENGTH);
  } else if (stepCount % steps == 2) {
    stepTo(STEP_LENGTH * -1, 0);
  } else {
    stepTo(0, STEP_LENGTH * -1);
    stepCount = -1;
  }
}
void sideStep() {
  stepCount++;
  if (stepCount % steps == 0) {
    stepTo(STEP_LENGTH, 0);
  } else if (stepCount % steps == 1) {
    stepTo(STEP_LENGTH * -1, 0);
  } else if (stepCount % steps == 2) {
    stepTo(STEP_LENGTH * -1, 0);
  } else {
    stepTo(STEP_LENGTH, 0);
    stepCount = -1;
  }
}
void updownStep() {
  stepCount++;
  if (stepCount % steps == 0) {
    stepTo(0, STEP_LENGTH);
  } else if (stepCount % steps == 1) {
    stepTo(0, STEP_LENGTH * -1);
  } else if (stepCount % steps == 2) {
    stepTo(0, STEP_LENGTH * -1);
  } else {
    stepTo(0, STEP_LENGTH);
    stepCount = -1;
  }
}
void randomStep() {
  float rad = random(360) * pi / 180;
  int x = cos(rad) * STEP_LENGTH;
  int y = sin(rad) * STEP_LENGTH;
  stepTo(x, y);
}

////////////////////////////////////////////////////////////
// Initialize
void setup() {
  // Initialize pins
  pinMode(OUTPUT_LED, OUTPUT); //LED

  // Start USB
  DigiMouse.begin();
}

////////////////////////////////////////////////////////////
// Main loop
void loop() {
  // LED heartbeat
  ledBlink();
  count++;
  if (count == maxCount) {
    count = 0;
  }

  if (waiting == waitFor) {
    // Switch mouse behavior
    switch (STEP_MODE) {
      case 1: // Box Step
        boxStep();
        break;
      case 2: // Side Step
        sideStep();
        break;
      case 3: // Updown Step
        updownStep();
        break;
      case 4: // Random Step
        randomStep();
        break;
      default: // LED on
        ledOn();
        break;
    }
    // Initialize waiting timer
    waiting = 0;
    waitFor = STEP_INTERVAL;
    // For confirming startup during STARTUP_INTERVAL * STARTUP_COUNT
    if (count < STARTUP_INTERVAL * STARTUP_COUNT) {
      waitFor = STARTUP_INTERVAL;
    }
    if (WAIT_RANDOM) {
      waitFor = random(1, waitFor);
    }
  } else {
    DigiMouse.delay(backInterval);
  }
  waiting++;

  // Mouse needs to be ran `update()`, `delay()` will wait and call it.
  DigiMouse.delay(heartbeat * 1000 - backInterval);
}

