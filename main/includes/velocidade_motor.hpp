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
#define circunferencia_do_motor 0.05135  // Circunferencia do motor em m
#define imas_motor 4                   // Define a quantidades de imas que o motor tem

/**
 * @class A classe a baixo é responsável por calcular a velocidade do motor com base na variacao
 * dos sinais do hall do motor e no calculo do intervalo de tempo desses sinais
 * Essa classe nao segue o mesmo padrão das outras, como a do Encoder, pois a função que ambas 
 * utilizam para perceber a variação de sinais, AttachInterrupt é tratada de forma diferente, 
 * onde, nesse código,a função só funciona se estiver associada a funções do escorpo/globais
 * Em vista disso, esse arquivo .hpp acabou sendo responsável apenas por gerar a impressao dos 
 * dados e inicializar o calculo
 * Caso exista alguma outra estratégia de código para contornar isso, o autor do código está aberto
 * a ajudar e a fazer as devidas alteracoes
*/

/**
 * @details Antes desse código ser commitado, ele foi devidamente compilado e executado, funcionando
 * normalmente, porém apresentando os problemas provindos do atraso do attach e da ineficiencia da
 * média móvel
*/
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
