#include <HTTPClient.h>
#include <Wire.h>
#include <SD.h>
#include <math.h>
#include <WiFi.h>
#include <TinyGSM.h>
#include <INA.h> // INA Library (by Zanshin)

// ########## -------------------- GPS DATA -------------------- ##########

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

// LilyGO T-SIM7000G Pinout
#define UART_BAUD   115200
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4
#define LED_PIN     12

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands
#define SerialAT  Serial1

TinyGsm modem(SerialAT);


// ########## -------------------- INA226 DATA -------------------- ##########

// Vamos calibrar o divisor de tensão e o sensor de corrente para o INA 226
// R5 = 20 kΩ
// R6 = 68 kΩ
// Divisor de tensão INA = R6/(R5 + R6) = 0,7727272... kΩ
// Divisor de tensão INA^-1 = (R5 + R6)/R6 = 1,29411764706
// fatorCorreçãoV = 1.01626

// Resistor do shunt = 1 mΩ
// FatorCorreçãoC = 0,482625

//#define R5 20000
//#define R6 68000
//#define valorShunt 0.001
//#define fatorCorrecaoV 1.01626
//#define fatorCorrecaoC 0.482625

#define fatorMili 0.001
#define fatorMicro 0.000001

INA_Class INA;


// ########## -------------------- LM35 DATA -------------------- ##########
#define ADC_VREF_mV    3300.0  // in millivolt
#define ADC_RESOLUTION 4096.0
#define AMPOP_OUT        32    // ESP32 pin connected to LM358P
#define GAIN              3.89 // AMPOP_OUT = LM35 output * GAIN


//-----------------------------------Replace with your network credentials----------------------------------------
// Replace with your network credentials
const char* ssid     = "SSID";
const char* password = "PASSWORD";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "SERVERNAME";

// Keep this API Key value to be compatible with the PHP code provided in the server.
String apiKeyValue = "APIKEYVALUE";
//----------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  // ------------------- INA226 SETUP --------------------
  INA.begin(80, 1000, 0x40);             // Begin calibration for an expected 80 Amps maximum current and for a 0.0O1hm resistor
  INA.setAveraging(10);                  // Average each reading n-times
  INA.setBusConversion(10000);           // Maximum conversion time 8.244ms
  INA.setShuntConversion(10000);         // Maximum conversion time 8.244ms
  INA.setMode(INA_MODE_CONTINUOUS_BOTH); // Bus/shunt measured continuously

  // ------------------- GPS SETUP --------------------
  SerialMon.println("Place your board outside to catch satelite signal");

  // Set LED OFF
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  //Turn on the modem
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(300);
  digitalWrite(PWR_PIN, LOW);

  delay(1000);

  // Set module baud rate and UART pins
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  if (!modem.restart()) {
    Serial.println("Failed to restart modem, attempting to continue without restarting");
  }//end_if

  // Print modem info
  String modemName = modem.getModemName();
  delay(500);
  SerialMon.println("Modem Name: " + modemName);

  String modemInfo = modem.getModemInfo();
  delay(500);
  SerialMon.println("Modem Info: " + modemInfo);

  Serial.println("All done with GPS! Starting HTTP config...");

  // ------------------- WIFI SETUP --------------------
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("WL_NOT_CONNECTED");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}


void loop() {
  // |_________________________________________________|
  // |____________________ INA 226  ___________________|
  // |_________________________________________________|
  // Variaveis do sensor
  int32_t rawCurrent = -666;
  float current_motor;
  uint16_t rawVoltage = 666;
  float voltage_battery;
  int32_t rawShunt = -666;
  float shunt_motor_INA;
  float power = 0;
  float consumption = 0;
  float consumoParcial = 0;

  //Variaveis de tempo
  long tempoAtual = 0;
  long tempoAnterior = 0;
  long tempoDelta = 0;

  float R5 = 2.2; // Resistência em kΩ
  float R6 = 2.2; // Resistência em kΩ
  double valorShunt = 0.001;
  double fatorCorrecaoV = 1.01626;
  double fatorCorrecaoC = 0.482625;

  rawVoltage = INA.getBusMilliVolts();
  voltage_battery = (float)rawVoltage * ((R5 + R6) / R6) * fatorCorrecaoV * fatorMili; // Esse valor que está sendo multiplicado pelo valor da tensão tem a função de calibrar o sensor.

  Serial.print("Tensão Bateria :");
  Serial.println(voltage_battery);

  // voltage_battery = (float)rawVoltage * 0.00131516;
  //  Vbat = Vina *      1.29411764706       *    1.01626    *   0.001
  //  Vbat = Vina * fatorDivisorTensao * fatorCorreçãoV * fatorMili

  // _______Sensor de Corrente INA___________ /

  rawCurrent = INA.getShuntMicroVolts();
  current_motor = (float)rawCurrent * (fatorMicro / valorShunt) * fatorCorrecaoC;

  Serial.print("Corrente Bateria :");
  Serial.println(current_motor);

  // _______Medição de potência e consumo INA___________ /

  power = (float)INA.getBusMicroWatts() * fatorMicro;
  power *= fatorCorrecaoV;

  Serial.print("Potencia Instantanea :");
  Serial.println(power);

  tempoAnterior = tempoAtual;
  tempoAtual = millis();
  tempoDelta = tempoAtual - tempoAnterior;
  consumoParcial = power * (tempoDelta) * fatorMili;
  consumption = consumption + consumoParcial;

  Serial.print("Consumo :");
  Serial.println(consumption);

  // -------------------------------------------------------------------------------------------------


  // |_________________________________________________|
  // |____________________ GPS 226  ___________________|
  // |_________________________________________________|
  // ---------- GPS data ----------
  float lat      = 0;
  float lng      = 0;
  float speed    = 0;
  float alt      = 0;
  int   vsat     = 0;
  int   usat     = 0;
  float accuracy = 0;
  int   year     = 0;
  int   month    = 0;
  int   day      = 0;
  int   hour     = 0;
  int   minutos  = 0;
  int   sec      = 0;
  String reading_time = "";

  // ---------- Collecting GPS Data ----------
    modem.sendAT("+SGPIO=0,4,1,1");
  if (modem.waitResponse(10000L) != 1) {
    SerialMon.println(" SGPIO=0,4,1,1 false ");
  }
  modem.enableGPS();

  for (int8_t i = 15; i; i--) {
    SerialMon.println("Requesting current GPS/GNSS/GLONASS location");
    if (modem.getGPS(&lat, &lng, &speed, &alt, &vsat, &usat, &accuracy,
                      &year, &month, &day, &hour, &minutos, &sec)) {
      String reading_time = String(year) + "-" + String(month) + "-" + String(day) + " " + String(hour) + ":" + String(minutos) + ":" + String(sec);
      SerialMon.println("Latitude: " + String(lat, 8) + "\tLongitude: " + String(lng, 8));
      SerialMon.println("Year: " + String(year) + "\tMonth: " + String(month) + "\tDay: " + String(day));
      SerialMon.println("Hour: " + String(hour) + "\tMinute: " + String(minutos) + "\tSecond: " + String(sec));
      SerialMon.println("Reading_time: " + reading_time);
      break;
    }
    else {
      SerialMon.println("Couldn't get GPS/GNSS/GLONASS location, retrying in 15s.");
      delay(15000);
    }
  }

  // -------------------------------------------------------------------------------------------------


  // |_________________________________________________|
  // |____________________ LM35 226  __________________|
  // |_________________________________________________|
  // ---------- LM35 data ----------
  int adcValAmpOp;
  int adcVal;
  float milliVolt;
  int celcius = 30;
  int farenheits;

  
  adcValAmpOp = analogRead(AMPOP_OUT);
  adcVal = adcValAmpOp / GAIN;
  milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  celcius = milliVolt / 10;
  farenheits = celcius * 9 / 5 + 32;

  // -------------------------------------------------------------------------------------------------


  // |_________________________________________________|
  // |___________________ SEND DATA  __________________|
  // |_________________________________________________|
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&lat=" + String(lat, 8) + "&lng=" + String(lng, 8) + 
                              "&celcius=" + String(celcius) + "&farenheits=" + String(farenheits) + 
                              "&voltage_battery=" + String(voltage_battery, 1) + 
                              "&current_motor=" + String(current_motor, 1) + "&power=" + String(power, 1) + 
                              "&consumption=" + String(consumption, 1) + 
                              "&reading_time=" + reading_time + "";

    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

  } else {
    Serial.println("WiFi Disconnected. Attempting to connect again");
    WiFi.begin(ssid, password);
    Serial.println("Connecting");

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("WL_NOT_CONNECTED");
    }

    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());;
  }
}
