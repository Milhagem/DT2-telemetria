/**
 * @file t-sim7000g.ino
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Versao base para ser aplicada o RTOS.
 * @version 0.0.1
 * @date 2023-07-15
 */

#include <Arduino.h>

#include "datalogger.hpp"
#include "lm35.hpp"
#include "gps.hpp"
#include "ina226.hpp"
#include "encoder.hpp"
#include "displayTFT.hpp"

Datalogger datalogger;
LM35 lm35;
GPS gps;
Encoder encoder;
DisplayTFT display;
Ina226 ina226;

// Insira abaixo o nome do arquivo que sera gerado
String nome_arquivo = "teste_20230715_21:00";

void setup() {
  Serial.begin(115200);
  
  datalogger.setupDatalogger();
  datalogger.abreArquivo("/"+nome_arquivo);

  gps.setupGPS();
  encoder.setupEncoder();
  /* Testes sendo realizados sem o DisplayTFT e Ina226. */
  display.setupDisplayTFT();
  ina226.setupINA226();

}

void loop() {
  lm35.atualizaLM35();
  lm35.imprimir();

  gps.atualizaGPS();
  gps.imprimir();

  int rpm = encoder.amostraVoltas();
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

  datalogger.concatenaArquivo("/"+nome_arquivo, new_data);

}
