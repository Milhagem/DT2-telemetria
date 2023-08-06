#ifndef LTECONNECTION_H
#define LTECONNECTION_H

#include "MODEM_CONFIG.hpp";
extern TinyGsm modem;

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]  = "zap.vivo.com.br";     // SET TO YOUR APN
const char gprsUser[] = "vivo";
const char gprsPass[] = "vivo";

class LTE_Connection {
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