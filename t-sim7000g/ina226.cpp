#include "ina226.hpp"

void ina226::setupINA226() {
  INA.begin(80, 1000, 0x40);             // Begin calibration for an expected 80 Amps maximum current and for a 0.0O1hm resistor
  INA.setAveraging(10);                  // Average each reading n-times
  INA.setBusConversion(10000);           // Maximum conversion time 8.244ms
  INA.setShuntConversion(10000);         // Maximum conversion time 8.244ms
  INA.setMode(INA_MODE_CONTINUOUS_BOTH); // Bus/shunt measured continuously

  this->tempoAtual = 0;
  this->tempoAnterior = 0;
  this->tempoDelta = 0;
}

void ina226::atualizaINA226() {
  // Atualiza tensao
  uint16_t raw_voltage = INA.getBusMilliVolts();;
  this->voltage_battery = (float)raw_voltage * ((R5 + R6) / R6) * fatorCorrecaoV * mili; // Esse valor que esta sendo multiplicado pelo valor da tensao tem a funcao de calibrar o sensor

  // Atualiza corrente
  int32_t raw_current = INA.getShuntMicroVolts();;
  this->current_motor = (float)raw_current * (micro / shunt) * fatorCorrecaoC; // Esse valor que esta sendo multiplicado pelo valor da corrente tem a funcao de calibrar o sensor

  // Atualiza potencia
  this->power = (float)INA.getBusMicroWatts() * micro;
  this->power *= fatorCorrecaoV;

  // Atualiza consumo
  tempoAnterior = tempoAtual;
  tempoAtual = millis();
  tempoDelta = tempoAtual - tempoAnterior;
  float consumoParcial = power * (tempoDelta) * mili;
  consumption = consumption + consumoParcial;
}

void ina226::imprimir() {
  Serial.print("Tensão Bateria: ");
  Serial.print(this->voltage_battery);
  Serial.println("V")
  Serial.print("Corrente Motor: ");
  Serial.print(this->current_motor);
  Serial.println("A")
  Serial.print("Potencia Instantânea: ");
  Serial.print(this->power);
  Serial.println("W")
  Serial.print("Consumo: ");
  Serial.print(this->consumption);
  Serial.println("unidades de energia")
}