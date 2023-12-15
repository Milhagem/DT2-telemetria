/**
 * @file gps.hpp
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Codigo gps da placa de telemetria do DT2.
 * @version 1.1
 * @date 2023-08-16
 * @details O código inclui o arquivo "MODEM_CONFIG.hpp" que são configurações relacionadas ao 
 *          modem GSM que são usadas neste código
 *          
 *          
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GPS_H
#define GPS_H

#include "MODEM_CONFIG.hpp"
extern TinyGsm modem;

class GPS {
private:
    float lat;
    float lon;
    float speed;
    float alt;
    int   vsat;
    int   usat;
    float accuracy;
    int   year;
    int   month;
    int   day;
    int   hour;
    int   min;
    int   sec;

public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o GPS
    */
    void setupGPS();
    /**
     * @brief Verifica disponibilidade e librera o GPS
     */
    void liberaGPS();
    /**
     * @brief Atualiza todos os atributos do GPS
     */
    void atualizaGPS();
    /**
     * @brief Imprime todos os atributos do GPS (lat, lon, speed, alt...) na saida Serial
    */
   
    void imprimir();

    String getLat() { return String(this->lat, 8); }
    String getLon() { return String(this->lon, 8); }

    /**
     * @return Timestamp no formato YYYYMMDD hh:mm:ss
    */
    String getTimestamp();

};
 
#endif
