#define MOISTURE_SENSOR_PIN 36
#define FLOW_METER_PIN      2
#define WATER_PUMP_PIN      14

struct FlowMeter {
  const uint8_t PIN;
  uint32_t pulses_count;
  float total_milli_litres;
  bool running;
  unsigned long last_running;
};

FlowMeter flow_meter = {FLOW_METER_PIN, 0, 0, false, 0};

void IRAM_ATTR pulseCounter() {
  flow_meter.pulses_count++;
  flow_meter.running = true;
}

float calibration_factor = 4.5;

int interval = 1000;
unsigned long last_time = 0;

#define MOISTURE_NONE 2660
#define MOISTURE_WET  930
#define IRRIGATION_THRESHOLD_PERCENTAGE 50
#define IRRIGATION_AMOUNT 1000


void setup() {
  Serial.begin(115200);

  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(FLOW_METER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(flow_meter.PIN), pulseCounter, FALLING);
}

void loop() {
  if ((millis() - last_time) > interval) {
    /*
    * moisture_sensor
    */
    int moisture_analog = analogRead(MOISTURE_SENSOR_PIN);
    Serial.println(moisture_analog);
    int moisture_percentage = map(moisture_analog, MOISTURE_NONE, MOISTURE_WET, 0, 100);
    if (moisture_percentage > 100) moisture_percentage = 100;
    if (moisture_percentage < 0) moisture_percentage = 0;

    Serial.print("Moisture = ");
    Serial.print(moisture_percentage);
    Serial.println("%");


    if (moisture_percentage < IRRIGATION_THRESHOLD_PERCENTAGE) {
      // start pomp
      Serial.println("Start pomp");
      digitalWrite(WATER_PUMP_PIN, HIGH);
    }


    /*
    * flow meter
    */
    if (flow_meter.running) {
      uint8_t pulse_count = flow_meter.pulses_count;
      float flow_rate = ((1000.0 / (millis() - flow_meter.last_running)) * pulse_count) / calibration_factor;
      Serial.print("Flow Rate: ");
      Serial.println(flow_rate);
      flow_meter.last_running = millis();
      flow_meter.running = false;

      float flow_milli_litres = (flow_rate / 60) * 1000;
      Serial.print("Flow Milli Litres: ");
      Serial.println(flow_milli_litres);
      flow_meter.total_milli_litres += flow_milli_litres;

      Serial.print("Total milli Litres: ");
      Serial.println(flow_meter.total_milli_litres);

      if (flow_meter.total_milli_litres >= IRRIGATION_AMOUNT) {
        flow_meter.total_milli_litres = 0;
        // stop pomp
        Serial.println("Stop pomp");
        digitalWrite(WATER_PUMP_PIN, LOW);
      }
    }

    last_time = millis();
  }
}
