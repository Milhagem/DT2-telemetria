//#define configUSE_TIME_SLICING 1
//#define configTICK_RATE_HZ 10

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

// LilyGO T-SIM7000G Pinout
#define UART_BAUD   115200
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4
#define LED_PIN     12

#include <TinyGSM.h>

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands
#define SerialAT  Serial1
TinyGsm modem(SerialAT);

#define TFT_BL   32            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)
#define TFT_RGB_ORDER TFT_BGR                      // color order Blue-Green-Red - for this specific display  
#include <TFT_eSPI.h>                              // hardware-specific library
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();                         // invoke custom library

// some principal color definitions
// RGB 565 color picker at https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0
#define SCALE0      0xC655                                                  // accent color for unused scale segments                                   
#define SCALE1      0x5DEE 
// circular scale color scheme  
#define BLUE2RED 1
#define GREEN2BLUE 2

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <INA.h> // INA Library (by Zanshin)
#include <SD.h>
#include <math.h>
#include "FS.h"
#include "SD_MMC.h"

#define ONE_BIT_MODE  true

//-----------------------------------Replace with your network credentials----------------------------------------
// Replace with your network credentials
const char* ssid     = "Milhagem";
const char* password = "ehomilhas";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://telemetria.milhagemufmg.com/post_data.php";

// Keep this API Key value to be compatible with the PHP code provided in the server.
String apiKeyValue = "tPmAT5Ab3j7F9";
//----------------------------------------------------------------------------------------------------------------
// Variaeis compartilhadas entre as threads
float lat_comp = 0;
float lng_comp = 0;
int celcius_comp = 0;
int farenheits_comp = 0;
float voltage_battery_comp = 0;
float current_motor_comp = 0;
float power_comp = 0;
float consumption_comp = 0;
String reading_time_comp = "1-1-1900";

//----------------------------------------------------------------------------------------------------------------
#define fatorMili 0.001
#define fatorMicro 0.000001
#define ADC_VREF_mV    3300.0  // in millivolt
#define ADC_RESOLUTION 4096.0
#define AMPOP_OUT        35    // ESP32 pin connected to LM358P
#define GAIN              3.89 // AMPOP_OUT = LM35 output * GAIN


// ---------- Display data  ----------
float gasto = 0;
double vel_media = 0;
double vel = 0;   
int gaugeposition_x = 20;  // these two variables govern the position
int gaugeposition_y = 20;  // of the square + gauge on the display

// ---------- Energy data  ----------
// Variaveis do sensor INA226
INA_Class INA; // Construct a power monitor object names "ina"
int32_t rawCurrent = -666;
float current_motor;
uint16_t rawVoltage = 666;
float voltage_battery;
int32_t rawShunt = -666;
float shunt_motor_INA;
float power = 0;
float aux_power = 0;
float consumption = 0;
float consumoParcial = 0;
//Variaveis de tempo
long tempoAtual = 0;
long tempoAnterior = 0;
long tempoDelta = 0;
 
// ---------- Encoder data ----------
#define MEASURE_PIN 25
const double WHEEL_CIRCUMFERANCE = 1.596;
const double pulses_per_revolution = 16;
const double fator_correcao = 0.7;
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

double rpm;
double wheel_diameter; 
double tempo_Inicio = 0;
double tempo_speedVelho = 0;
double tempo_speedAtual = 0;
double tempo_dif = 0;
double tempo_total = 0;
double speed = 0;
double speed_mpm = 0;
double speed_velho = 0;
double average_speed = 0;
double distancia_trecho = 0;
double distancia_total = 0;
double ms_to_min =0.00001666666;
bool andou = 0;

// ---------- GPS data ----------
float lat      = 0;
float lng      = 0;
float velocidade   = 0;
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
String reading_time = "1-1-1900";

// ---------- Temperature data ----------
int adcValAmpOp;
int adcVal;
float milliVolt;
int celcius;
int farenheits;
//----------------------------------------------------------------------------------------------------------------

SemaphoreHandle_t SemaphoreBuffer;
//----------------------------------------------------------------------------------------------------------------

// interrupt handler is triggered when magnet enters or leaves field
void magnet_detection_changed() {
  // check if magnet is nearby
  if (digitalRead(MEASURE_PIN) == HIGH) {
    // increase revolutions
    samples[nextSample].revolutions++;
  }
}

void setup(){
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);

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

  INA.begin(80, 1000, 0x40);             // Begin calibration for an expected 80 Amps maximum current and for a 0.0O1hm resistor
  INA.setAveraging(10);                  // Average each reading n-times
  INA.setBusConversion(10000);           // Maximum conversion time 8.244ms
  INA.setShuntConversion(10000);         // Maximum conversion time 8.244ms
  INA.setMode(INA_MODE_CONTINUOUS_BOTH); // Bus/shunt measured continuously

  SerialMon.println("Place your board outside to catch satelite signal");

  tft.init();  
  tft.setRotation (2);       // display in portrait
  tft.fillScreen (BLACK); 
  //EH O MILHAS
  tft.setCursor (165,300); tft.setTextSize (1);
  tft.print ("EH O MILHAS"); //ntj


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

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("WL_NOT_CONNECTED");
  }//end_while
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Create the semaphore
  SemaphoreBuffer = xSemaphoreCreateBinary();

  //Set the semaphore
  xSemaphoreGive(SemaphoreBuffer);

  // Create the tasks
  xTaskCreatePinnedToCore(EnvioDeDadosTask, "Envio De Dados Task", 10000, NULL, 1, NULL, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(INATask, "INA Task", 10000, NULL, 1, NULL, APP_CPU_NUM);
  xTaskCreatePinnedToCore(TemperaturaTask, "Temperatura Task", 10000, NULL, 1, NULL, APP_CPU_NUM);
  xTaskCreatePinnedToCore(
    GPSTask,          // Task function
    "GPS Task",       // Task name
    10000,           // Stack size
    NULL,            // Task parameters
    1,               // Priority
    NULL,            // Task handle
    APP_CPU_NUM               // Core number (0 or 1)
  );
  // Notify that all tasks have been created (lock Serial with mutex)
  Serial.println("All tasks created");


    pinMode(2, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);
    pinMode(15, INPUT_PULLUP);
    
    if(!SD_MMC.begin("/sdcard", ONE_BIT_MODE)){
        Serial.println("Card Mount Failed");
        return;
    }
    
    File myFile = SD_MMC.open("/RamChu.txt", FILE_WRITE);
}

//----------------------------------------------------------------------------------------------------------------

void TemperaturaTask(void *pvParameters) {
  while (true) {
    // read the ADC value from the temperature OpAmp
    adcValAmpOp = analogRead(AMPOP_OUT);
    // converts de ADC value read from the OpAmp into the LM35 original value
    adcVal = adcValAmpOp / GAIN;
    // convert the ADC value to voltage in millivolt
    milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
    // convert the voltage to the temperature in °C
    
    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);
      celcius_comp = milliVolt / 10;
      // convert the °C to °F
      farenheits_comp = celcius * 9 / 5 + 32;
    xSemaphoreGive(SemaphoreBuffer);
    
    Serial.print("Celsius :");
    Serial.println(celcius);
  }//while
}//end Tempertura Task
//----------------------------------------------------------------------------------------------------------------

void INATask(void *pvParameters) {
  while (true) {
    // ______Sensor de Tensão INA__________ /
    float R5 = 20; // Resistência em kΩ
    float R6 = 68; // Resistência em kΩ
    double valorShunt = 0.001;
    double fatorCorrecaoV = 1.01626;
    double fatorCorrecaoC = 0.97000;
    
    
    rawVoltage = INA.getBusMilliVolts();
    // _______Sensor de Corrente INA___________ /
    rawCurrent = INA.getShuntMicroVolts();
    // _______Medição de potência e consumo INA___________ /
    aux_power = (float)INA.getBusMicroWatts() * fatorMicro;
    aux_power *= fatorCorrecaoV;    
    tempoAnterior = tempoAtual;
    tempoAtual = millis();
    tempoDelta = tempoAtual - tempoAnterior;
    consumoParcial = aux_power * (tempoDelta) * fatorMili;
    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);
      voltage_battery_comp = (float)rawVoltage * ((R5 + R6) / R6) * fatorCorrecaoV * fatorMili; // Esse valor que está sendo multiplicado pelo valor da tensão tem a função de calibrar o sensor.
      current_motor_comp = (float)rawCurrent * (fatorMicro / valorShunt) * fatorCorrecaoC;
      power_comp = aux_power;
      consumption_comp = consumption_comp + consumoParcial;
    xSemaphoreGive(SemaphoreBuffer);
    
    Serial.print("Tensão Bateria :");
    Serial.print("Corrente Bateria :");
    Serial.println(current_motor);
    Serial.println(voltage_battery);
    Serial.print("Potencia Instantanea :");
    Serial.println(power);
    Serial.print("Consumo :");
    Serial.println(consumption);
  }//end_while
}//end_INATask
//----------------------------------------------------------------------------------------------------------------

void GPSTask(void *pvParameters) {
  while (true) {
    // Set SIM7000G GPIO4 HIGH ,turn on GPS power
    // CMD:AT+SGPIO=0,4,1,1
    // Only in version 20200415 is there a function to control GPS power
    modem.sendAT("+SGPIO=0,4,1,1");
    if (modem.waitResponse(10000L) != 1) {
       SerialMon.println(" SGPIO=0,4,1,1 false ");
    }//end_if
    modem.enableGPS();

    // Take the semaphore to access the shared resource
    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);
      for (int8_t i = 15; i; i--) {
      //SerialMon.println("Requesting current GPS/GNSS/GLONASS location");
        if (modem.getGPS(&lat_comp, &lng_comp, &velocidade, &alt, &vsat, &usat, &accuracy,
                          &year, &month, &day, &hour, &minutos, &sec)) {
          String reading_time_comp = String(year) + "-" + String(month) + "-" + String(day) + " " + String(hour) + ":" + String(minutos) + ":" + String(sec);
          SerialMon.println("Latitude: " + String(lat_comp, 8) + "\tLongitude: " + String(lng_comp, 8));
          SerialMon.println("Year: " + String(year) + "\tMonth: " + String(month) + "\tDay: " + String(day));
          SerialMon.println("Hour: " + String(hour) + "\tMinute: " + String(minutos) + "\tSecond: " + String(sec));
          SerialMon.println("Reading_time: " + reading_time);
         break;
        }//end_if
        else {
          // SerialMon.println("Couldn't get GPS/GNSS/GLONASS location, retrying in 1ms.");
          vTaskDelay(1);
        }//end_else
      }//end_for
    // Release the semaphore
    xSemaphoreGive(SemaphoreBuffer);
    
    Serial.print("Tempo task GPS :");
  }//end while
}//end GPS task
//----------------------------------------------------------------------------------------------------------------

void EnvioDeDadosTask(void *pvParameters) {
  while (true) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);
        lat = lat_comp;
        lng = lng_comp;
        celcius = celcius_comp;
        farenheits = farenheits_comp;
        voltage_battery = voltage_battery_comp;
        current_motor = current_motor_comp;
        power = power_comp;
        consumption = consumption_comp;
        reading_time = reading_time_comp;        
      xSemaphoreGive(SemaphoreBuffer);   

      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue +"&rpm=" + String(rpm,1) + "&speed=" + String(speed, 2) + "&average_speed=" + String(average_speed, 2) + "&wheel_diameter=" + String(wheel_diameter, 2) +
                              "&lat=" + String(lat, 8) + "&lng=" + String(lng, 8) + 
                              "&celcius=" + String(celcius) + "&farenheits=" + String(farenheits) + 
                              "&voltage_battery=" + String(voltage_battery, 1) + "&current_motor=" + String(current_motor, 1) + "&power=" + String(power, 1) + "&consumption=" + String(consumption, 1) + 
                              "&reading_time=" + reading_time + "";

      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }//end_if
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }//end_else
      // Free resources
      http.end();
    }//end_if
    else {
      Serial.println("WiFi Disconnected. Attempting to connect again");
      WiFi.begin(ssid, password);
      Serial.println("Connecting");
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("WL_NOT_CONNECTED");
      }//end_while
      Serial.println("");
      Serial.print("Connected to WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());;
    }//end_else
  }//end while
}//end EnvioDeDados
//----------------------------------------------------------------------------------------------------------------

int ringMeter(double value, double vmin, double vmax, int x, int y, int r, char *units, byte scheme);

void loop() {
//Cria o primeiro arco-íris
  //VELOCIMETRO
  vel = rpm;
  gasto = consumption;
  vel_media = average_speed;
  
  int radius = 80; //upper ring radius
  ringMeter (vel,0,60,(gaugeposition_x+20),(gaugeposition_y+57),radius,"Km/h",GREEN2BLUE);
      
  //Cria o texto do GASTO
  //JOULÍMETRO - Gasto acumulado;
  int xpos = 55, ypos = 25; //text position
  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (xpos,ypos); tft.setTextSize (2);                              // units position relative to scale
  tft.print ("Gasto: ");
  tft.setCursor (xpos+72,ypos); tft.setTextSize (2);                           // units position relative to scale
  tft.print (gasto);  
  tft.setCursor (xpos+115,ypos); tft.setTextSize (2);                          // units position relative to scale
  tft.print ("J");

  //Cria o texto do Vel Med
  //VEL MED - Velocidade media;
  xpos = 40, ypos = 265; //text position
  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (xpos-10,ypos); tft.setTextSize (2);                              // units position relative to scale
  tft.print ("Vel Med: ");
  tft.setCursor (xpos+85,ypos); tft.setTextSize (2);                           // units position relative to scale
  tft.print (vel_media);  
  tft.setCursor (xpos+125,ypos); tft.setTextSize (2);                          // units position relative to scale
  tft.print ("KM/H");  

  Serial.print(", rps=");
  Serial.print(rps);
  Serial.print(", rpm=");
  rpm = rps * 60;
  Serial.print(rpm);
  speed = (rpm * WHEEL_CIRCUMFERANCE * 60.0 / 1000.0) * fator_correcao;
  Serial.print(", speed= ");
   Serial.print(speed); 
  Serial.println();

  if(speed != 0 && speed_velho == 0 && andou == 0){
    Serial.println("Começou a andar");
    tempo_Inicio = millis();
    tempo_speedVelho = millis();
    tempo_speedAtual= millis();
    speed_velho = speed;
    andou = 1;
  }
  else if(andou == 1){
    tempo_speedAtual = millis();
    tempo_total = tempo_speedAtual - tempo_Inicio;
    tempo_dif = tempo_speedAtual - tempo_speedVelho;
    distancia_trecho = speed * tempo_dif / 3600000;
    distancia_total = (distancia_trecho + distancia_total)/1000.0;
    average_speed = distancia_total/ ((tempo_total * ms_to_min)/60.0);
    speed_velho = speed;
    tempo_speedVelho = tempo_speedAtual;
  }
  Serial.print("velocidade media :");
  Serial.println(average_speed);
  Serial.print("distancia_total :");
  Serial.println(distancia_total);
  Serial.print("tempo_total :");
  Serial.println(tempo_total/1000);
  
  wheel_diameter = WHEEL_CIRCUMFERANCE;

  File myFile = SD_MMC.open("/RamChu.txt", FILE_APPEND);
  if (myFile){
    //Serial.println("FILE OK !!!");
  } else {
    //Serial.println("FILE NOT OK !!!");
  }
    myFile.print(lat);
    myFile.print(',');
    myFile.print(lng);
    myFile.print(',');
    myFile.print(celcius);
    myFile.print(',');
    myFile.print(farenheits);
    myFile.print(',');
    myFile.print(voltage_battery);
    myFile.print(',');
    myFile.print(current_motor);
    myFile.print(',');
    myFile.print(power);  
    myFile.print(',');
    myFile.print(consumption);
    myFile.print(',');
    myFile.print(reading_time);
}

// ################################################################################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// ################################################################################################################################

int ringMeter(double value, double vmin, double vmax, int x, int y, int r, char *units, byte scheme) {
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option

  x += r; y += r;                                                             // calculate coordinates of center of ring
  int w = r / 3;                                                              // width of outer ring is 1/4 of radius
  int angle = 150;                                                            // half the sweep angle of the meter (300 degrees)
  int v = map(value, vmin, vmax, -angle, angle);                              // map the value to an angle v
  byte seg = 3;                                                               // segments are 3 degrees wide = 100 segments for 300 degrees
  byte inc = 6;                                                               // draw segments every 3 degrees, increase to 6 for segmented ring
  int colour = BLUE;                                                          // variable to save "value" text color from scheme and set default


  for (int i = -angle+inc/2; i < angle-inc/2; i += inc)                       // draw color blocks every increment degrees
    {           
    float sx = cos((i - 90) * 0.0174532925);                                 // calculate pair of coordinates for segment start
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;

    float sx2 = cos((i + seg - 90) * 0.0174532925);                          // salculate pair of coordinates for segment end
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;

    if (i < v) 
    {                                                                     // fill in coloured segments with 2 triangles
      switch (scheme)
      {
        case 1: colour = rainbow(map(i, -angle, angle, 0, 127)); break;  // full spectrum blue to red
        case 2: colour = rainbow(map(i, -angle, angle, 70, 0)); break;   // verde para azul (velocidade dt2)
        default: colour = BLUE; break;                                   // fixed color
      }
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
    }else                                                                    // fill in blank segments
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, SCALE1);                // color of the unoccupied ring scale 
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, SCALE0);                // color of the unoccupied ring scale
    }
  }

  char buf [10];                                                              // convert value to a string
  byte len = 3; if (value > 999) len = 4;
  dtostrf (value, len, 0, buf);
  buf[len] = ' '; buf[len] = 0;                                               // add blanking space and terminator, helps to centre text too!

  tft.setTextSize (2);
  if (value>9)
  {
    tft.setTextColor (WHITE,BLACK);
    tft.setCursor (x-25,y-10);tft.setTextSize (5);
    tft.print (buf);
  }

  if(value<10)
  {
    tft.setTextColor (WHITE,BLACK);
    tft.setCursor (x-25,y-10);tft.setTextSize (5);
    tft.print (buf);
  }

  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (x-23,y+55);tft.setTextSize (2);                              // units position relative to scale
  tft.print (units);                                                          // units display = celsius                                        
  return x + r;                                                               // calculate and return right hand side x coordinate
}

// ################################################################################################################################
// Return a 16 bit rainbow colour
// ################################################################################################################################

unsigned int rainbow(byte value) {                                             // value is expected to be in range 0-127
                                                                               // value is converted to a spectrum color from 0 = blue through to 127 = red
  byte red = 0;                                                               // red is the top 5 bits of a 16 bit colour value
  byte green = 0;                                                             // green is the middle 6 bits
  byte blue = 0;                                                              // blue is the bottom 5 bits
  byte quadrant = value / 32;

  if (quadrant == 0)
  {
    blue = 0; //31;
    green = 2 * (value % 32);
    red = 31; //0;
  }
  if (quadrant == 1)
  {
    blue = 0; //31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2)
  {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3)
  {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}

// ################################################################################################################################
// Return a value in range -1 to +1 for a given phase angle in degrees
// ################################################################################################################################

float sineWave(int phase) {
  return sin(phase * 0.0174532925);
}

