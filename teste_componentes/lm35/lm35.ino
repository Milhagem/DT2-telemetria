#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0
#define AMPOP_OUT        32   // ESP32 pin GIOP36 (ADC0) connected to LM35
#define GAIN             3.89

void setup() {
  Serial.begin(115200);
}

void loop() {
  // read the ADC value from the temperature OpAmp
  int adcValAmpOp = analogRead(AMPOP_OUT);
  // converts de ADC value read from the OpAmp into the LM35 original value
  int adcVal = adcValAmpOp / GAIN;
  // convert the ADC value to voltage in millivolt
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  // convert the voltage to the temperature in °C
  int celcius = milliVolt / 10;
  // convert the °C to °F
  int farenheits = celcius * 9 / 5 + 32;

  // print the temperature in the Serial Monitor:
  Serial.print("Temperature: ");
  Serial.print(celcius);   // print the temperature in °C
  Serial.print("°C");
  Serial.print("  ~  "); // separator between °C and °F
  Serial.print(farenheits);   // print the temperature in °F
  Serial.println("°F");

  delay(500);
}
