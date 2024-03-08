/**
 * @file encoder.hpp
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief 
 * @date 2023-08-09
 * @details 
 * definimos samples como numero de pontos medidos no disco de freio, time_interval para
 * intervalo de tempo amostra de pulsos, e avelocade minima para trocar o metodo de amotraVoltas
 * @copyright Copyright (c) 2023
 * 
 */


#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "setup_esp.hpp"

class Encoder {
private:
    double speed;           // km/h
    double average_speed;   // km/h
    double distancia_total; // km

    /**
     * Variaveis que serao usadas em void calculaVelocidade(double rps, double wheel_diameter)
    */
    double tempo_inicio;      // segundos
    double tempo_speed_old;   // segundos
    double tempo_speed_atual; // segundos
    double tempo_delta;       // segundos (diferenca de tempo)
    double tempo_total;       // segundos
    double speed_old;         // m/s
    double distancia_trecho;  // metros
    bool ja_andou;
        
public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o Encoder
    */
    void setupEncoder();
    /**
     * @brief Calcula voltas totais e voltas por MINUTO da roda
     * @details A cada TIME_INTERVAL, conta o numero de pulsos e calcula a velocidade
     * 
     * @return rpm
    */
    double amostraVoltasTimeInterval();
    /**
     * @brief Calcula voltas totais e voltas por MINUTO da roda
     * @details A cada numero de SAMPLES, analisa o intervalo de tempo gasto e calcula a velocidade
     * 
     * @return rpm
    */
    double amostraVoltasSamples();
     /**
     * @brief Calcula a velocidade com base nas rotacoes por segundo e WHEEL_CIRCUMFERANCE
     * 
     * @param rpm Rotacoes por minuto
    */
    void calculaVelocidade(double rpm);
    /**
     * @brief Imprime os atributos do Encoder (speed, average_speed, distancia_total e tempo_total) na saida Serial
    */
    void imprimir();
    // km/h
    String getSpeed() { return String(this->speed); }
    // km/h
    String getAverageSpeed() { return String(this->average_speed); }
}; 

#endif
