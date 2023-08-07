#ifndef LTECONNECTION_H
#define LTECONNECTION_H

#include "../includes/MODEM_CONFIG.hpp"
extern TinyGsm modem;
extern TinyGsmClient client;

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]  = "zap.vivo.com.br";     // SET TO YOUR APN
const char gprsUser[] = "vivo";
const char gprsPass[] = "vivo";

// Server details
const char server[]   = "www.telemetria.milhagemufmg.com";
const char resource[] = "/post_data.php";
const int  port       = 80;

class LTE_Connection {
private:

public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar a conexao LTE
     * 
     */
    void setupLTE();
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g APOS a funcao setupLTE() para que seja estabelecida a conexao GPRS
     *
     */
    void connectGRPS(const char* apn, const char* gprsUser, const char* gprsPass);
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g APOS a funcao connectGRPS() para que seja estabelecida a conexao com o servidor desejado
     * 
     */
    void connectServer(const char* server, const char* resource, const int  port);

    /**
     * @brief Executa uma requisicao HTTP Post e imprime na tela o codigo de resposta
     * @details A requisicao HTTP eh executada para o recurso informado. No caso, eh o arquivo /post_data.php. A requisicao deve estar no formato -www-form-urlencoded, por exemplo: "api_key=" + String("123456789") + "&lat=" + String(-19.98765432) + "&lng=" + String(-45.09876543) + ""
     * 
     * @param server www.telemetria.milhagemufmg.com
     * @param resourse /post_data.php
     * @param port 80
     * @param postData Dados a serem inseridos no banco, no formato x-www-form-urlencoded
     * 
     */
    void postRequest(const char* server, const char* resource, const int  port, String postData);

    void modemPowerOn();
    void modemPowerOff();
    void modemRestart();
};


#endif
