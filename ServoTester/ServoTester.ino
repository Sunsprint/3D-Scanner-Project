#include <Servo.h>

#define SERVO 3
#define DEC_BUTTON 6
#define INC_BUTTON 7
Servo servo;

#define MIN_RANGE 10
#define MAX_RANGE 170
#define INCREMENT = 30;
int currentPos = 0;

boolean lastDecPressed = false;
boolean currentDecPressed = false;
boolean lastIncPressed = false;
boolean currentIncPressed = false;

void setup() {
  pinMode(DEC_BUTTON, INPUT);
  pinMode(INC_BUTTON, INPUT);
  servo.attach(SERVO);
  Serial.begin(9600);
  Serial.println("Setup done.");
}

void loop() {
  lastDecPressed = currentDecPressed;
  currentDecPressed = digitalRead(DEC_BUTTON);
  if (!lastDecPressed && currentDecPressed) {
    currentPos -= INCREMENT;
    currentPos = constrain(currentPos, MIN_RANGE, MAX_RANGE);
    Serial.println(currentPos);
  }
  
  lastIncPressed = currentIncPressed;
  currentIncPressed = digitalRead(INC_BUTTON);
  if (!lastIncPressed && currentIncPressed) {
    currentPos += INCREMENT;
    currentPos = constrain(currentPos, MIN_RANGE, MAX_RANGE);
    Serial.println(currentPos);
  }
  servo.write(currentPos);

  delay(20);
}
