#define SENSOR A5

void setup() {
  pinMode(SENSOR, INPUT);
  Serial.begin(9600);
  Serial.println("Setup done.");
}

void loop() {
  Serial.print("Output: ");
  int output = analogRead(SENSOR);
  Serial.println(output);
  delay(500);
}
