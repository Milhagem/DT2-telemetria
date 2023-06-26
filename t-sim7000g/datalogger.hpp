#ifndef DATALOGGER_H
#define DATALOGGER_H

#include "FS.h"
#include "SD_MMC.h"

#define SD_MISO 2 
#define SD_CS   13
#define SD_MOSI 15
#define SD_SCLK 14
#define PWR_PIN 4
#define LED_PIN 12

#define ONE_BIT_MODE  true

class datalogger {
private:
    File meu_arquivo;
    
public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o Datalogger
    */
    void dataloggerSetup();
        /**
     * @brief Abre o arquivo passado como parametro se ele existir. Se nao, cria o arquivo
     * 
     * @param nome_arquivo nome do arquivo
    */
    void abreArquivo(String nome_arquivo);
    void concatenaArquivo(const char * path, const char * message);
};

#endif 