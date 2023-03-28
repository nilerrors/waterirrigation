int pumpPin = 14;

void setup() {
  pinMode(pumpPin, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  Serial.println("Pump on");
  digitalWrite(pumpPin, HIGH);
  delay(3000);
  Serial.println("Pump off");
  digitalWrite(pumpPin, LOW);
  delay(3000);
}
