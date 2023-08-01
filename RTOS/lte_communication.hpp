#ifndef LTECOMMUNICATION_H
#define LTECOMMUNICATION_H

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#define SerialAT Serial1
// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

#include <TinyGsmClient.h>

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]  = "zap.vivo.com.br";     // SET TO YOUR APN
const char gprsUser[] = "vivo";
const char gprsPass[] = "vivo";

// LilyGO T-SIM7000G Pinout
#define UART_BAUD           115200
#define PIN_DTR             25
#define PIN_TX              27
#define PIN_RX              26
#define PWR_PIN             4
#define LED_PIN             12

class LTE_Communication {
private:

public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar a conexao LTE
     * 
     */
    void setupLTE();

    void modemPowerOn();
    void modemPowerOff();
    void modemRestart();
};


#endif