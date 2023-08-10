/**
 * @file t-sim7000g.ino
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Versao base para ser aplicada o RTOS.
 * @version 0.0.1
 * @date 2023-08-09
 */

#include <Arduino.h>

#include "includes/MODEM_CONFIG.hpp"
#include "includes/lte_connection.hpp"
#include "includes/datalogger.hpp"
#include "includes/lm35.hpp"
#include "includes/gps.hpp"
#include "includes/ina226.hpp"
#include "includes/encoder.hpp"
#include "includes/displayTFT.hpp"

Datalogger datalogger;
LTE_Connection conexao;
LM35 lm35;
GPS gps;
Encoder encoder;
DisplayTFT display;
Ina226 ina226;

// Insira abaixo o nome do arquivo que sera gerado
String path = "/teste_20230827_23:47.txt";

// Insira abaixo a apiKeyValue para insercao no Banco de dados
String api_key_value = "tPmAT5Ab3j7F9";

void setup() {
  Serial.begin(115200);
  
  // DSetup datalogger
  datalogger.setupDatalogger();
  datalogger.abreArquivo(path);

  // Setup conexao LTE (4G)
  conexao.connectGRPS(apn, gprsUser, gprsPass);
  conexao.connectServer(server, resource, port);

  // Setup GPS
  gps.setupGPS();

  // Setup sensor de velocidade (encoder optico)
  encoder.setupEncoder();

  // Setup displayTFT
  display.setupDisplayTFT();

  // Setup sensor de tensao e corrente (INA226)
  ina226.setupINA226();

}

void loop() {
  // Sensor de temperatura LM35
  lm35.atualizaLM35();
  lm35.imprimir();

  // GPS
  gps.atualizaGPS();
  gps.imprimir();

  // Sensor de velocidade (Encoder Optico)
  int rpm = encoder.amostraVoltasTimeInverval();
  encoder.calculaVelocidade(rpm);
  encoder.imprimir();

  // DisplayTFT
  display.ringMeter(encoder.getSpeed(), 0, 60, (GAUGEPOSITION_X+20), (GAUGEPOSITION_Y+57), RADIUS, "Km/h", GREEN2BLUE);
  display.mostraVelocidadeMedia(encoder.getAverageSpeed());
  display.mostraConsumo(ina226.getConsumption());

  // Sensor de tensao e corrente INA226
  ina226.atualizaINA226();
  ina226.imprimir();

  // Gravando no Datalogger
  String log_data = gps.getTimestamp()+","+gps.getLat()+","+gps.getLon()+","+
                    ina226.getVoltage()+","+ina226.getCurrent()+","+ina226.getPower()+","+ina226.getConsumption()+","+
                    encoder.getSpeed()+","+encoder.getAverageSpeed()+","+
                    lm35.getTemperatura();
  datalogger.concatenaArquivo(path, log_data);

  // Inserindo no Banco de Dados na WEB
  String post_data = "api_key=" + api_key_value + "&rpm=" + String(encoder.amostraVoltasSamples()) + "&speed=" +  encoder.getSpeed() + "&average_speed=" + encoder.getAverageSpeed() + "&wheel_diameter=" + WHEEL_CIRCUMFERANCE +
                     "&lat=" + gps.getLat() + "&lng=" + gps.getLon() + 
                     "&celcius=" + String(74) + "&farenheits=" + String(30) + 
                     "&voltage_battery=" + ina226.getVoltage() + "&current_motor=" + ina226.getCurrent() + "&power=" + ina226.getPower() + "&consumption=" + ina226.getConsumption() + 
                     "&reading_time=" + gps.getTimestamp() + "";

  conexao.postRequest(server, resource, post_data);
}
