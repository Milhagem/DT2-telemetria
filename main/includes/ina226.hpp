/**
 * @file ina226.hpp
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Subclasse sensor de tensao e corrente, tensao da bateria e corrente no motor
 * @details
 * INA226


 *  SOBRE TENSAO:
 *  INA RECEBE NO MAXIMO DE 36V, ENTRETANTO A BATERIA É DE 42V, POR ISSO PRECISAMOS DO DIVISOR DE TENSAO COM R5 E R6

 *  SOBRE CORRENTE:
 *  O SHUNT EH UM RESISTOR DE RESISTENCIA MUITO BAIXA E O INA CALCULA A CORRENTE QUE PASSA POR ELE (QUE EH 
 *  A CORRENTE DO MOTOR) COM BASE NA 
 *  QUEDA NA LEI DE OHM E NA QUEDA DE TENSAO DO SHUNT

 *  SOBRE FATOR DE CORRECAO:
 *  O INA NAO EH MUITO PRECISO. OS FATORES DE fatorCorrecaoV E fatorCorrecaoC SERVEM APENAS PARA CALIBRA-LO

 * @version 0.1
 * @date 2023-08-07
 * 
 * 
 */
#ifndef INA226_H
#define INA226_H

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

class Ina226 {
private:
    INA_Class INA;

    
    float current_motor;
    float voltage_battery;
    float power;
    float consumption;

    // Variaveis de tempo para calculo de power e consumption
    long tempoAtual;
    long tempoAnterior;
    long tempoDelta;

public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o INA226
    */
    void setupINA226();
    /**
     * @brief Atualiza os atributos current_motor, voltage_battery, power e consumption;
    */
    void atualizaINA226();
     /**
     * @brief Imprime todosos atributos do INA226 (current_motor, voltage_battery...) na saida Serial;
    */
    void imprimir();

    String getCurrent() { return String(this->current_motor); }
    String getVoltage() { return String(this->voltage_battery); }
    String getPower() { return String(this->power); }
    String getConsumption() { return String(this->consumption); }

};

#endif
