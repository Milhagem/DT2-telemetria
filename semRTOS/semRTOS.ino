/**
 * @file t-sim7000g.ino
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Versao base para ser aplicada o RTOS.
 * @version 0.0.1
 * @date 2023-07-15
 */

#include <Arduino.h>

#include "../RTOS/includes/MODEM_CONFIG.hpp"
#include "../RTOS/includes/lte_connection.hpp"
#include "../RTOS/includes/datalogger.hpp"
#include "../RTOS/includes/lm35.hpp"
#include "../RTOS/includes/gps.hpp"
#include "../RTOS/includes/ina226.hpp"
#include "../RTOS/includes/encoder.hpp"
#include "../RTOS/includes/displayTFT.hpp"

Datalogger datalogger;
LTE_Connection conexao;
LM35 lm35;
GPS gps;
Encoder encoder;
/* Testes sem o DisplayTFT */
// DisplayTFT display;
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
  /* Testes sem o DisplayTFT. */
  //display.setupDisplayTFT();

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

  /**
   * Codigo do DisplayTFT 
   * [...]
   */

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

  conexao.postRequest(server, resource, port, post_data);
}
