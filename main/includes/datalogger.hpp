#ifndef DATALOGGER_H
#define DATALOGGER_H

#include "FS.h"
#include "SD_MMC.h"
#include "setup_esp.hpp"


class Datalogger {
private:
    File meu_arquivo;
    
public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o Datalogger
    */
    void setupDatalogger();
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
    void concatenaArquivo(String path, String data);

    /* Implementar estas funcoes... */
    // String lePrimeiraLinha(File file);
    // String leUltimaLinha(File file);
};

#endif 
