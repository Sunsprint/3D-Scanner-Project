#include <Servo.h>

// Sensor
#define DISTANCE_SENSOR A5
#define DISTANCE_CUTOFF 40
#define DIST_FROM_SENSOR 20


// Servos
#define YAW_SERVO 5
#define PITCH_SERVO 6
Servo yaw;
Servo pitch;

#define MIN_YAW 45
#define MAX_YAW 135
#define YAW_STEP 5
#define YAW_OFFSET 90 // Number to subtract when calculating points

#define MIN_PITCH 45
#define MAX_PITCH 90
#define PITCH_STEP 5
#define YAW_OFFSET 90 // Number to subtract when calculating points

int yawIndex = 0;
int pitchIndex = 0;


// Info storage
#define NUM_YAW (MAX_YAW - MIN_YAW) / YAW_STEP + 1
#define NUM_PITCH (MAX_PITCH - MIN_PITCH) / PITCH_STEP + 1
boolean pointExists[NUM_YAW][NUM_PITCH];
unsigned long lastUpdateTime;
boolean done = false;

// Given a value from the distance sensor, return the distance in cm
double analogToDistance(int analog) {
  return (log(analog) - 6.44124) / -0.01675;
}
// Given a value from the distance sensor, return whether the scanner sees something
boolean doesPointExist(int analog) {
  return analogToDistance(analog) < DISTANCE_CUTOFF;
}


void setup() {
  pinMode(DISTANCE_SENSOR, INPUT);
  
  yaw.attach(YAW_SERVO);
  pitch.attach(PITCH_SERVO);
  yawIndex = 0;
  pitchIndex = 0;

  lastUpdateTime = millis();
  done = false;
  
  Serial.begin(9600);
  Serial.println("Setup done.");
}

void loop() {
  if (!done) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime > 500) {
      int analog = analogRead(DISTANCE_SENSOR);
      pointExists[yawIndex][pitchIndex] = doesPointExist(analog);
      yawIndex++;
      if (yawIndex >= NUM_YAW) {
        yawIndex = 0;
        pitchIndex++;
        if (pitchIndex >= NUM_PITCH) {
          done = true;
          Serial.print("[");
          for (yawIndex = 0; yawIndex < NUM_YAW; yawIndex++) {
            for (pitchIndex = 0; pitchIndex < NUM_PITCH; pitchIndex++) {
              if (pointExists[yawIndex][pitchIndex]) {
                int yaw = MIN_YAW + YAW_STEP * yawIndex;
                int pitch = MIN_PITCH + PITCH_STEP * pitchIndex;
                double z = DIST_FROM_SENSOR;
                double x = z * tan(yaw);
                double y = z * tan(pitch);
                Serial.print("[");
                Serial.print(x);
                Serial.print(", ");
                Serial.print(y);
                Serial.print(", ");
                Serial.print(z);
                Serial.print("]");
              }
            }
          }
          Serial.println("]");
        }
      }
    }
  }
  delay(20);
}
