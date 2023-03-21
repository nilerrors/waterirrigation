#define MOISTURE_SENSOR_PIN 18
#define FLOW_METER_PIN      15
#define WATER_PUMP_PIN      12

float calibrationFactor = 4.5;

int total_milli_liters = 0;
volatile byte pulse_count;

void IRAM_ATTR pulseCounter() {
  pulse_count++;
}

int interval = 1000;
unsigned long lastTime = 0;


void setup() {
  Serial.begin(115200);

  pinMode(MOISTURE_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_PIN), pulseCounter, FALLING);
}

void loop() {
  if ((millis() - lastTime) > interval) {
    flowRate = ((1000.0 / (millis() - lastTime)) * pulse_count) / calibrationFactor;
    lastTime = millis();

    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    Serial.print("Totaal Litres: ");
    Serial.println(totalMilliLitres / 1000);
  }
}
