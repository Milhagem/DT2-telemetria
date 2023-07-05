#include <Arduino.h>
#include "datalogger.hpp"
#include "lm35.hpp"
#include "ina226.hpp"

Datalogger datalogger;
LM35 lm35;
Ina226 ina226;

// Escreva o nome do arquivo que sera gravado
String nome_arquivo = "teste_20230703.txt";

void setup() {
  delay(2000);
  Serial.begin(115200);
  
  ina226.setupINA226();

  datalogger.dataloggerSetup();
  datalogger.abreArquivo("/"+nome_arquivo);
}

void loop() {
  lm35.atualizaLM35();
  lm35.imprimir();

  ina226.atualizaINA226();
  ina226.imprimir();
  
  String dados = "Temperatura: " + String(lm35.getTemperatura()) + " Tensao: " + String(ina226.getVoltage()) + " Corrente: " + String(ina226.getCurrent());
  
  datalogger.concatenaArquivo("/"+nome_arquivo, dados);

  delay(5000);
}
