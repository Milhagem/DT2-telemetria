// //**
//  * @file velocidade_motor.hpp
//  * @author Milhagem UFMG (contatomilhagem@ufmg.com)
//  * @brief
//  * @version 1.0
//  * @date 2023-10-25
//  * @details
//  * @copyright Copyright (c) 2023
//  *
//  */
#ifndef VELOCIDADE_MOTOR_H
#define VELOCIDADE_MOTOR_H
#include <math.h>
#include <Arduino.h>
#define hall_motor 34                    // Define o pino que saira as informacoes do hall
#define circunferencia_do_motor 0.1634  // Circunferencia do motor em m
#define imas_motor 4                   // Define a quantidades de imas que o motor tem
/// @brief
class Velocidade_Motor {
private:

public:
    /**
     * @brief Essa funcao faz as configuracoes iniciais
    */
   void motor_setup ();
   /**
    * @brief Imprime o valor da velocidade na saida do serial
   */
  void imprimir ();
};
#endif
