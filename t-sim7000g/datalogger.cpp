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

void Datalogger::abreArquivo(String path){
    this->meu_arquivo = SD_MMC.open("/"+ path + ".txt", FILE_WRITE);
    if (this->meu_arquivo){
        Serial.println("FILE OK !!!");
    } else {
        Serial.println("FILE NOT OK !!!");
    }

    // Confere se o arquivo criado possui cabecalho. Se nao tiver, adiciona
    String cabecalho = "timestamp,lat,lon,voltage,current,power,consumption,speed,average_speed,temperatura\n";
    String primeira_linha = this->lePrimeiraLinha(this->meu_arquivo);
    if (primeira_linha.length() == 0) {
        this->meu_arquivo.print(cabecalho);
    }

    this->meu_arquivo.close();
}

void Datalogger::concatenaArquivo(String path, String* dados){
    Serial.printf("Appending to file: %s\n", path);

    this->meu_arquivo = SD_MMC.open(path, FILE_APPEND);
    if(!this->meu_arquivo ) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if(this->meu_arquivo.print(*dados)) {
        Serial.println("Message appended:");
        // Imprime a ultima linha do arquivo
        Serial.println(this->leUltimaLinha(this->meu_arquivo));
    } else {
        Serial.println("Append failed");
    }
    this->meu_arquivo .close();
}

String Datalogger::lePrimeiraLinha(File file) {
    String firstLine = file.readStringUntil('\n');

    file.close();

    if (firstLine.length() > 0) {
        Serial.print("A primeira linha do arquivo é: ");
        Serial.println(firstLine);
        return firstLine;
    } else {
        Serial.println("O arquivo está vazio");
        return firstLine;
    }
}

String Datalogger::leUltimaLinha(File file) {
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
        Serial.print("A última linha do arquivo é: ");
        Serial.println(lastLine);
        return lastLine;
    } else {
        Serial.println("O arquivo está vazio");
        return lastLine;
    }
}