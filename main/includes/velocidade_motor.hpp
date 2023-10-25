/**
 * @file velocidade_motor.hpp
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief 
 * @version 1.0
 * @date 2023-10-25
 * @details
 * @copyright Copyright (c) 2023
 * 
 */

#progama once
#ifndef VELOCIDADE_MOTOR_H
#define VELOCIDADE_MOTOR_H

#include <SD.h>
#include <math.h>

#include <Arduino.h>

#define hall_motor 4 //define o pino que saira as informacoes do hall
#define circunferencia_do_motor 0.3267 // esse dado tem que ser validado
#define imas_motor 4 //define a quantidades de halls que o motor tem

/// @brief 
class Velocidade_Motor {
private:
    int hall_motor;
    float circunferencia_do_motor;
    int imas_motor;
    
    long hall_valor_motor; //estado logico do sensor
    long contagem_motor; //conta o numero de voltas na roda/ numero de imas que passam pelo sensor

    float velocidade_instantanea_motor; //velocidade do motor em RPM
    volatile float diferenca_pulsos_motor; //diferenca entre os pulsos atual e o anterior =~ periodo
    int estado_ant_motor; //conta o estado logico anterior do sensor hall

    unsigned long tempo_atual; //tempo em milissegundos medido no loop atual
    volatile unsigned long tempo_anterior1_motor; // (momento anterior ao pulso anterior)
    volatile unsigned long tempo_anterior_motor; //atualiza toda vez que um pulso eh detectado (momento anterior ao pulso atual)
    volatile int conta_motor; //controla quando o calculo da velocidade eh realizado

public:
    /**
     * @brief Essa funcao calcula o valor da velocidade do motor
    */
   void calculo_velocidade ();
   /**
    * @brief Essa funcao calcula o intervalo de tempo da diferenca dos pulsos do hall
    * Ela serve para o calculo da velocidade
   */
  void calculo_intervalo_hall();
    /**
     * @brief Essa funcao faz as configuracoes iniciais
    */
   void motor_setup ();
};

#endif
