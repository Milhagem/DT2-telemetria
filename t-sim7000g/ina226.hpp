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

class ina226 {
private:
    INA_Class INA;

    float current_motor;
    float voltage_battery;
    float power;
    float consumption;

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
     * @brief Imprime todosos atributos na saida Serial;
    */
    void imprimir();

}

#endif