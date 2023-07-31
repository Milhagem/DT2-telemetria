#ifndef GPS_H
#define GPS_H

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#include <TinyGsmClient.h>

// LilyGO T-SIM7000G Pinout
#define UART_BAUD   115200
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define LED_PIN     12

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands
#define SerialAT  Serial1

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

    String getTimestamp();

};
 
#endif