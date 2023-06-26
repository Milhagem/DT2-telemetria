#include <Arduino.h>

#include "lm35.hpp"
#include "ina226.hpp"
#include "gps.hpp"
#include "httpclient.hpp"

// Coloque suas credenciais
const char* ssid = "";
const char* password = "";
const char* serverName;
String apiKeyValue;

LM35 lm35;
Ina226 ina226;
GPS gps;
httpclient envioDeDados(ssid, password, serverName, apiKeyValue);

void setup() {
  Serial.begin(115200);

  ina226.setupINA226();
  gps.setupGPS();
  envioDeDados.wifiSetup(ssid, password);
}

void loop() {
  lm35.medeTemperatura();
  lm35.imprimir();

  ina226.atualizaINA226();
  ina226.imprimir();
  
  gps.atualizaGPS();
  gps.imprimir();

}
