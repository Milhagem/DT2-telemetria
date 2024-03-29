#include "datalogger.hpp"

void Datalogger::setupDatalogger(){

    pinMode(SD_MISO, INPUT_PULLUP);
    pinMode(SD_CS, INPUT_PULLUP);
    pinMode(SD_MOSI, INPUT_PULLUP);
    pinMode(PWR_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, INPUT_PULLUP);
    
    if(!SD_MMC.begin("/sdcard", ONE_BIT_MODE)){
        Serial.println("Card Mount Failed");
        return;
    } else {
        Serial.println("Card Mount SUCCEED");
    }
}

void Datalogger::abreArquivo(String path){
    this->meu_arquivo = SD_MMC.open(path, FILE_READ);

    if(this->meu_arquivo.size() == 0) {
      Serial.println(path+" não existe. Criando e adicionando cabecalho...");
      
      String cabecalho = "timestamp,lat,lon,voltage,current,power,consumption,speed,average_speed,temperatura";
      this->meu_arquivo = SD_MMC.open(path, FILE_WRITE);
      this->meu_arquivo.println(cabecalho);
    } else {
      Serial.println("Abrindo "+path+ " para concatenacao...")  ;
      this->meu_arquivo = SD_MMC.open(path, FILE_APPEND);
    }

    if (this->meu_arquivo){
        Serial.println("FILE OK !!!");
    } else {
        Serial.println("FILE NOT OK !!!");
    }
    this->meu_arquivo.close();
}


void Datalogger::concatenaArquivo(String path, String data) {
    //Serial.print("Appending to file: ");
    //Serial.println(path);

    this->meu_arquivo = SD_MMC.open(path, FILE_APPEND);

    if(!this->meu_arquivo ) {
        Serial.println("Failed to open file for appending");
        return;
    }

    if(this->meu_arquivo) {
        this->meu_arquivo.println(data);
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }

    this->meu_arquivo.close();
}

// @camposouza Nao sei se essa funcao funciona
/* String Datalogger::lePrimeiraLinha(File file) {
    String firstLine = file.readStringUntil('\n');

    file.close();

    if (firstLine.length() > 0) {
       //  Serial.print("A primeira linha do arquivo é: ");
        Serial.println(firstLine);
        return firstLine;
    } else {
        Serial.println("O arquivo está vazio");
        return "";
    }
} */

// @camposouza Nao sei se essa funcao funciona
/* String Datalogger::leUltimaLinha(File file) {
    String lastLine;
    String currentLine;

    while (file.available()) {
        currentLine = file.readStringUntil('\n');
        if (currentLine.length() > 0) {
        lastLine = currentLine;
        }
    }

    file.close();

    if (lastLine.length() > 0) {
        // Serial.print("A última linha do arquivo é: ");
        Serial.println(lastLine);
        return lastLine;
    } else {
        Serial.println("O arquivo está vazio");
        return "";
    }
} */
