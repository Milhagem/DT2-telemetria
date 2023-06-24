/* https://gist.github.com/joshnuss/52a2dae98c4037ef70954432ee798fda
 * Using digital hall effect sensor SENS-M-10 (purchased at Abra)
 * 
 * MCU Board: ESP2-WROOM-32
 */

#define MEASURE_PIN 2
#define WHEEL_CIRCUMFERANCE 1.596/12
#define SAMPLES 25


typedef struct {
  uint16_t ms;
  uint16_t revolutions;
} sample_t;

volatile uint8_t nextSample = 0;
volatile sample_t samples[SAMPLES] = {};

hw_timer_t * timer = NULL;

// temp vars for computing totals
volatile sample_t* pSample;
uint8_t n;
uint8_t totalRevolutions;
uint8_t totalSamples;
uint16_t minMs;
uint16_t maxMs;
double rps;

// interrupt handler is triggered every 100ms
void IRAM_ATTR timer_interrupt() {
  // compute total revolutions and revolutions per second
  totalRevolutions = 0;
  totalSamples = 0;
  minMs = 0;
  maxMs = 0;
  rps = 0;

  // iterate thru samples
  for (int i=0;i<SAMPLES;i++) {
    n = (nextSample+i) % SAMPLES;
    pSample = &samples[n];

    if (pSample->ms > 0) {
      // good sample
      totalSamples++;
      totalRevolutions += pSample->revolutions;

      minMs = std::min(minMs, (uint16_t)pSample->ms);
      maxMs = std::max(maxMs, (uint16_t)pSample->ms);
    }
  }

  // compute rps
  if (totalSamples > 0) {
    rps = totalRevolutions * 1.0 / totalSamples;
  }

  // prepare for a new sample
  if (nextSample >= SAMPLES-1) {
    nextSample = 0;
  } else {
    nextSample++;
  }

  // clear old sample data
  samples[nextSample].ms = millis();
  samples[nextSample].revolutions = 0;
}

// interrupt handler is triggered when magnet enters or leaves field
void magnet_detection_changed() {
  // check if magnet is nearby
  if (digitalRead(MEASURE_PIN) == HIGH) {
    // increase revolutions
    samples[nextSample].revolutions++;
  }
}


void setup() {
  Serial.begin(115200);
  
  pinMode(MEASURE_PIN, INPUT);

  // prescaler 80 is for 80MHz clock. One tick of the timer is 1us
  timer = timerBegin(0, 80, true); 

  // attach interrupt handler
  timerAttachInterrupt(timer, &timer_interrupt, true); 

  // trigger timer every 100ms (100,000us)
  timerAlarmWrite(timer, 100000, true); 

  // enable the timer
  timerAlarmEnable(timer); 

  // trigger function when hall effect sensor changes (goes low->high or high->low)
  attachInterrupt(MEASURE_PIN, magnet_detection_changed, CHANGE);
}

void loop() {
  // output calculations for debug
  Serial.print("samples=");
  Serial.print(totalSamples);
  Serial.print(", revolutions=");
  Serial.print(totalRevolutions);
  Serial.print(", rps=");
  Serial.print(rps);
  Serial.print(", rpm=");
  Serial.print(rps * 60 * 0.333);
  Serial.print(", speed=");
  Serial.print(rps * 60 * 0.333 * WHEEL_CIRCUMFERANCE);  
  Serial.println();

  delay(1000);
}
