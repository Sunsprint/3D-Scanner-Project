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
#define MAX_YAW 105
#define YAW_STEP 5

#define MIN_PITCH 15
#define MAX_PITCH 60
#define PITCH_STEP 5

int yawIndex = 0;
int pitchIndex = 0;
int indexDirection = 1;

#define WAIT_TIME 500


// Info storage
#define NUM_READS 3
#define NUM_YAW (MAX_YAW - MIN_YAW) / YAW_STEP + 1
#define NUM_PITCH (MAX_PITCH - MIN_PITCH) / PITCH_STEP + 1
boolean pointExists[NUM_YAW][NUM_PITCH];
double distances[NUM_YAW][NUM_PITCH];
unsigned long lastUpdateTime;
boolean done = false;

// Given a value from the distance sensor, return the distance in cm
double analogToDistance(int analog) {
  return (log(analog) - 6.44124) / -0.01675;
}
// Given a distance from the distance sensor, return whether the scanner sees something
boolean doesPointExist(double distance) {
  return distance < DISTANCE_CUTOFF;
}


void setup() {
  // Distance sensor setup
  pinMode(DISTANCE_SENSOR, INPUT);

  // Servo setup
  yaw.attach(YAW_SERVO);
  yaw.write(MIN_YAW);
  yawIndex = 0;
  pitch.attach(PITCH_SERVO);
  pitch.write(MIN_PITCH);
  pitchIndex = 0;
  indexDirection = 1;

  // Info setup
  lastUpdateTime = millis();
  done = false;

  // General setup
  Serial.begin(9600);
  Serial.println("Setup done.");
}

void loop() {
  // If finished scan, do nothing
  if (!done) {
    unsigned long currentTime = millis();
    
    // Allow servo time to move and sensor to take reading
    if (currentTime - lastUpdateTime > WAIT_TIME) {
      lastUpdateTime = currentTime;

      // See if point is there
      double sum = 0;
      for (int reading = 0; reading < NUM_READS; reading++) {
      int analog = analogRead(DISTANCE_SENSOR);
        sum += analogToDistance(analog);
        delay(50);
      }
      double finalDistance = sum / NUM_READS;
      distances[yawIndex][pitchIndex] = finalDistance;
      pointExists[yawIndex][pitchIndex] = doesPointExist(finalDistance);
      

      // Log info
      Serial.print("Yaw ");
      Serial.print(MIN_YAW + yawIndex * YAW_STEP);
      Serial.print(" and pitch ");
      Serial.print(MIN_PITCH + pitchIndex * PITCH_STEP);
      if (pointExists[yawIndex][pitchIndex]) {
        Serial.print(" exists.");
      } else {
        Serial.print(" does not exist.");
      }
      Serial.print("Distance: ");
      Serial.println(finalDistance);

      // Increment position
      yawIndex += indexDirection;
      if (yawIndex >= NUM_YAW || yawIndex < 0) {
        yawIndex = constrain(yawIndex, 0, NUM_YAW - 1);
        indexDirection = -indexDirection;
        pitchIndex++;
        if (pitchIndex >= NUM_PITCH) {
          done = true;

          // Print out general info to send to python
          Serial.print("{'minYaw': ");
          Serial.print(MIN_YAW);
          Serial.print(", 'maxYaw': ");
          Serial.print(MAX_YAW);
          Serial.print(", 'yawStep': ");
          Serial.print(YAW_STEP);
          Serial.print(", 'minPitch': ");
          Serial.print(MIN_PITCH);
          Serial.print(", 'maxPitch': ");
          Serial.print(MAX_PITCH);
          Serial.print(", 'pitchStep': ");
          Serial.print(PITCH_STEP);
          Serial.print(", 'distances': [");

          // Print out data for each point
          for (yawIndex = 0; yawIndex < NUM_YAW; yawIndex++) {
            Serial.print("[");
            for (pitchIndex = 0; pitchIndex < NUM_PITCH; pitchIndex++) {
              Serial.print(distances[yawIndex][pitchIndex]);
              if (pitchIndex < NUM_PITCH - 1) {
                Serial.print(", ");
              }
            }
            Serial.print("]");
            if (yawIndex < NUM_YAW - 1) {
              Serial.print(", ");
            }
          }
          Serial.println("]}");
        }
      }

      // Update servo position
      yaw.write(MIN_YAW + yawIndex * YAW_STEP);
      pitch.write(MIN_PITCH + pitchIndex * PITCH_STEP);
    }
  }
}
