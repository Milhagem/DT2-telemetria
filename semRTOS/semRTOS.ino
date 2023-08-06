/**
 * @file t-sim7000g.ino
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Versao base para ser aplicada o RTOS.
 * @version 0.0.1
 * @date 2023-07-15
 */

#include <Arduino.h>

#include "../RTOS/includes/datalogger.hpp"
#include "../RTOS/includes/lm35.hpp"
#include "../RTOS/includes/gps.hpp"
#include "../RTOS/includes/ina226.hpp"
#include "../RTOS/includes/encoder.hpp"
#include "../RTOS/includes/displayTFT.hpp"

Datalogger datalogger;
LM35 lm35;
GPS gps;
Encoder encoder;
/* Testes sem o DisplayTFT */
// DisplayTFT display;
Ina226 ina226;

// Insira abaixo o nome do arquivo que sera gerado
String path = "/teste_20230827_23:47.txt";

void setup() {
  Serial.begin(115200);
  
  datalogger.setupDatalogger();
  datalogger.abreArquivo(path);

  gps.setupGPS();
  encoder.setupEncoder();
  /* Testes sem o DisplayTFT. */
  //display.setupDisplayTFT();
  ina226.setupINA226();

}

void loop() {
  lm35.atualizaLM35();
  lm35.imprimir();

  gps.atualizaGPS();
  gps.imprimir();

  int rpm = encoder.amostraVoltasTimeInverval();
  encoder.calculaVelocidade(rpm);
  encoder.imprimir();

  /**
   * Codigo do DisplayTFT 
   * [...]
   */

  ina226.atualizaINA226();
  ina226.imprimir();

  String new_data = gps.getTimestamp()+","+gps.getLat()+","+gps.getLon()+","+
                    ina226.getVoltage()+","+ina226.getCurrent()+","+ina226.getPower()+","+ina226.getConsumption()+","+
                    encoder.getSpeed()+","+encoder.getAverageSpeed()+","+
                    lm35.getTemperatura();

  datalogger.concatenaArquivo(path, new_data);

}
