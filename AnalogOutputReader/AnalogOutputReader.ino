#define SENSOR A5

void setup() {
  pinMode(SENSOR, OUTPUT);
  Serial.begin(9600);
  Serial.println("Setup done.");
}

void loop() {
  Serial.print("Output: ");
  Serial.println(analogRead(SENSOR));
  delay(500);
}
