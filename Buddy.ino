#include <Servo.h>

// Pin Assignments
const int touchPin = 3;
const int redPin = 5;
const int yellowPin = 6;
const int greenPin = 9;
const int servoPin = 11;

// Tap Tracking
unsigned long lastTapTime = 0;
unsigned long touchStartTime = 0;
int tapCount = 0;
bool touchActive = false;

// Servo Motor
Servo flagServo;

void setup() {
  pinMode(touchPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  // Turn off all LEDs initially
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);

  flagServo.attach(servoPin);
  flagServo.write(0);  // Start at 0 degrees

  Serial.begin(9600);
}

void loop() {
  int touchVal = digitalRead(touchPin);

  // Touch press detected
  if (touchVal == HIGH && !touchActive) {
    touchActive = true;
    touchStartTime = millis();
  }

  // Touch released
  if (touchVal == LOW && touchActive) {
    touchActive = false;
    unsigned long pressDuration = millis() - touchStartTime;

    if (pressDuration >= 1000) {
      // Long press → Rotate Servo
      Serial.println("Long Press Detected → Rotating Servo");
      rotateServo();
      tapCount = 0; // Clear tap sequence
    } else {
      // Short press (tap)
      if (millis() - lastTapTime < 800) {
        tapCount++;
      } else {
        tapCount = 1;
      }
      lastTapTime = millis();
    }
  }

  // Evaluate tap count after timeout
  if (tapCount > 0 && millis() - lastTapTime > 900) {
    handleTaps(tapCount);
    tapCount = 0;
  }
}

// Handle LED signals based on taps
void handleTaps(int count) {
  // Reset LEDs
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);

  if (count == 1) {
    Serial.println("1 Tap → Help Needed (Red)");
    digitalWrite(redPin, HIGH);
  } else if (count == 2) {
    Serial.println("2 Taps → Confused (Yellow)");
    digitalWrite(yellowPin, HIGH);
  } else if (count == 3) {
    Serial.println("3 Taps → All Good (Green)");
    digitalWrite(greenPin, HIGH);
  } else {
    Serial.println("Invalid Tap Count");
  }

  delay(1500); // Keep LED on visibly
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
}

// Raise and lower servo flag
void rotateServo() {
  flagServo.write(90); // Raise flag
  delay(1500);
  flagServo.write(0);  // Lower flag
  delay(500);
}
