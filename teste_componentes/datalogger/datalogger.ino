#include "FS.h"
#include "SD_MMC.h"

#define SD_MISO 2 
#define SD_CS   13
#define SD_MOSI 15
#define SD_SCLK 14
#define PWR_PIN 4
#define LED_PIN 12

#define ONE_BIT_MODE  true

void setup(){
    Serial.begin(115200);

    pinMode(SD_MISO, INPUT_PULLUP);
    pinMode(SD_CS, INPUT_PULLUP);
    pinMode(SD_MOSI, INPUT_PULLUP);
    pinMode(PWR_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, INPUT_PULLUP);
    
    if(!SD_MMC.begin("/sdcard", ONE_BIT_MODE)){
        Serial.println("Card Mount Failed");
        return;
    } else {
        Serial.println("Card Mount SUCCEED")
    }
    
    File myFile = SD_MMC.open("/arquivo_teste.txt", FILE_WRITE);
}

void loop(){
    File myFile = SD_MMC.open("/arquivo_teste.txt", FILE_APPEND);
    if (myFile){
        Serial.println("FILE OK !!!");
    } else {
        Serial.println("FILE NOT OK !!!");
        return -1
    }
    for (int i = 0; i < 10; i++) {
        myFile.println("Atencao chegou chatuba hein");
        myFile.println("Vamo esculachar");
    }

}