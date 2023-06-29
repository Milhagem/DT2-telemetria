#include "datalogger.hpp"

void Datalogger::dataloggerSetup(){

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

void Datalogger::abreArquivo(String nome_arquivo){
    this->meu_arquivo = SD_MMC.open("/"+ nome_arquivo + ".txt", FILE_WRITE);
    if (this->meu_arquivo){
        Serial.println("FILE OK !!!");
    } else {
        Serial.println("FILE NOT OK !!!");
    }
    this->meu_arquivo.close();
}

void Datalogger::concatenaArquivo(const char * path, String* dados){
    Serial.printf("Appending to file: %s\n", path);

    this->meu_arquivo = SD_MMC.open(path, FILE_APPEND);
    if(!this->meu_arquivo ) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if(this->meu_arquivo.print(*dados)) {
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    this->meu_arquivo .close();
}
