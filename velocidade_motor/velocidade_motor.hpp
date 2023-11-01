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

#ifndef VELOCIDADE_MOTOR_H
#define VELOCIDADE_MOTOR_H

#include <math.h>

#include <Arduino.h>

#define hall_motor 5                    // Define o pino que saira as informacoes do hall
#define circunferencia_do_motor 0.1634  // Circunferencia do motor em m
#define imas_motor 4                   // Define a quantidades de imas que o motor tem

/// @brief 
class Velocidade_Motor {
private:
    
    long contagem_motor; // Serve para contar se houve uma sequencia efetiva de, por exemplo, 1 0 1

    float velocidade_instantanea_motor;      // Armazena o valor de velocidade em km/h
    volatile float diferenca_pulsos_motor;  // 1/4 do Periodo ou o delta t
    int estado_ant_motor;                  // Inicia o estado do motor com 0

    unsigned long tempo_atual;                     // Tempo em milissegundos medido no operador da velocidade
    volatile unsigned long tempo_anterior1_motor; // (Momento anterior ao pulso anterior)
    volatile unsigned long tempo_anterior_motor; // Atualiza toda vez que um pulso eh detectado (momento anterior ao pulso atual)
    volatile int conta_motor;                   // Controla quando o calculo da velocidade eh realizado

public:
    /**
     * @brief Essa funcao calcula o valor da velocidade do motor em km/h
    */
   void calculo_velocidade ();
   /**
    * @brief Essa funcao calcula o intervalo de tempo da diferenca dos pulsos do hall
    * Ela serve para permitir o calculo da velocidade
   */
  void calculo_intervalo_hall();
    /**
     * @brief Essa funcao faz as configuracoes iniciais
    */
   void motor_setup ();
   /**
    * @brief Imprime o valor da velocidade na saida do serial
   */
  void imprimir ();
  static void hallInterruptHandler();
  static Velocidade_Motor* instance;

};

#endif