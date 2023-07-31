#ifndef DATALOGGER_H
#define DATALOGGER_H

/**
 * @file datalogger.hpp
 * @author camposouza(guilhermesouza0911@ufmg.br)
 * @brief  Datalogger Class header file
 * @version 0.1
 * @date 2023-07-15
 * 
 * @details Classe responsavel por criar um arquivo.txt e gravar nele todos os dados lidos pelos sensores.
 * Esta classe possui um metodo setupDatalogger() que deve ser chamada na funcao main.ino para funcionar corretamente,
 * alem de um metodo abreArquivo() que tambem deve ser chamado no setup(). Feito isso,
 * Carregue o codigo na placa t-sim7000g sem o cartao SD. Em seguida, coloque o cartao SD e aperte o botao RST (reset).
 * Nesse ponto, o cartao deve ser montado com sucesso.
 */

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
    void setupDatalogger();
    /**
     * @brief Abre o arquivo passado no path se ele existir. Se nao, cria o arquivo e adiciona o cabecalho. Esta funcao deve ser chamada uma unica vez, no  setup() do t-sim7000g
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
