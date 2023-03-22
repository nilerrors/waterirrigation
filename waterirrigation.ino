#define MOISTURE_SENSOR_PIN 14
#define FLOW_METER_PIN      25
#define WATER_PUMP_PIN      35

float calibration_factor = 4.5;

int total_milli_litres = 0;
volatile byte pulse_count;

void IRAM_ATTR pulseCounter() {
  pulse_count++;
}

int interval = 1000;
unsigned long last_time = 0;


void setup() {
  Serial.begin(115200);

  pinMode(FLOW_METER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_PIN), pulseCounter, FALLING);
}

void loop() {
  if ((millis() - last_time) > interval) {
    /*
    * moisture_sensor
    */
    int moisture_analog = analogRead(MOISTURE_SENSOR_PIN);
    Serial.println(moisture_analog);
    int moisture_percentage = (100 - ((moisture_analog / 4095.00) * 100));
    // int moisture_percentage = map(moisture_analog * 3.3 / 4095, 1.2, 2.5, 0, 100);

    Serial.print("Moisture = ");
    Serial.print(moisture_percentage);
    Serial.println("%");


    /*
    * flow meter
    */
    // int flow_rate = ((1000.0 / (millis() - last_time)) * pulse_count) / calibration_factor;

    // int flow_milli_litres = (flow_rate / 60) * 1000;
    // total_milli_litres += flow_milli_litres;

    // Serial.print("Total Litres: ");
    // Serial.println(total_milli_litres / 1000);

    last_time = millis();
  }
}
