#include <INA.h>

/**
 *  Vamos calibrar o divisor de tensao e o sensor de corrente para o INA 226
 *  Divisor de tensão INA = R6/(R5 + R6) = 0,7727272... kΩ
 *  Divisor de tensão INA^-1 = (R5 + R6)/R6 = 1,29411764706 kΩ
*/ 

#define R5              20       // kΩ
#define R6              68       // kΩ
#define shunt           0.001    //  Ω
#define fatorCorrecaoV  1.01626  // fator para calibrar a tensao
#define fatorCorrecaoC  0.97000  // fator para calibrar a corrente

#define mili  0.001     // 10^-3 (m)
#define micro 0.000001  // 10^-6 (µ)

INA_Class INA;

void setup() {
  Serial.begin(115200);

  // ------------------- INA226 SETUP --------------------
  INA.begin(80, 1000, 0x40);             // Begin calibration for an expected 80 Amps maximum current and for a 0.0O1hm resistor
  INA.setAveraging(10);                  // Average each reading n-times
  INA.setBusConversion(10000);           // Maximum conversion time 8.244ms
  INA.setShuntConversion(10000);         // Maximum conversion time 8.244ms
  INA.setMode(INA_MODE_CONTINUOUS_BOTH); // Bus/shunt measured continuously
}

void loop() {

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

  // Variaveis de tempo
  long tempoAtual = 0;
  long tempoAnterior = 0;
  long tempoDelta = 0;

  // _______Sensor de Tensao INA___________


  rawVoltage = INA.getBusMilliVolts();
  voltage_battery = (float)rawVoltage * ((R5 + R6) / R6) * fatorCorrecaoV * mili; // Esse valor que está sendo multiplicado pelo valor da tensão tem a função de calibrar o sensor.

  Serial.print("Tensão Bateria :");
  Serial.println(voltage_battery);

  // _______Sensor de Corrente INA___________

  rawCurrent = INA.getShuntMicroVolts();
  current_motor = (float)rawCurrent * (micro / shunt) * fatorCorrecaoC;

  Serial.print("Corrente Bateria :");
  Serial.println(current_motor);

  // _______Medição de potência e consumo INA___________

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
}