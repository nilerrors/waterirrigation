int sensorPin = 2;
volatile long pulse;
float volume;
void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(sensorPin), increase, RISING);
}
void loop() {
  volume = 2.663 * pulse;
  Serial.print(volume);
  Serial.println(" mL");
  delay(500);
}
void increase() {
  pulse++;
}
