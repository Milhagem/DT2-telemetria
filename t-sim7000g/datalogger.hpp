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

class Datalogger {
private:
    File meu_arquivo;
    
public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o Datalogger
    */
    void dataloggerSetup();
    /**
     * @brief Abre o arquivo passado no path se ele existir. Se nao, cria o arquivo e adiciona o cabecalho
     * 
     * @param path caminho para o arquivo. Ex: /nome_arquivo.txt
    */
    void abreArquivo(String path);
    /**
     * @brief Adiciona mensagem ao final do arquivo
     * 
     * @param path caminho para o arquivo. Ex: /nome_arquivo.txt
     * @param message mensagem a ser adicionada
    */
    void concatenaArquivo(String path, String message);
    String lePrimeiraLinha(File file);
    String leUltimaLinha(File file);
};

#endif 
