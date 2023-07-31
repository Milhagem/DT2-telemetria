/**
Versão mais recente do código de telemetria do DT2
--------------------
Autores:
    

--------------------
Funcionalidades:
    GPS
    Datalogger
    Encoder
    INA226
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "datalogger.hpp"
#include "encoder.hpp"
#include "gps.hpp"
#include "ina226.hpp"


// ---------------------------------------------------------------------------------------------------
Encoder encoder;
GPS gps;
Datalogger datalogger;
Ina226 ina;

// ---------------------------------------------------------------------------------------------------
SemaphoreHandle_t SemaphoreBuffer;
const String path = "/teste_RTOS.txt";

// ---------------------------------------------------------------------------------------------------
void setup() {

    Serial.begin(115200);

    // Wait a moment to start 
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Setups
    gps.setupGPS();
    datalogger.setupDatalogger();
    datalogger.abreArquivo(path);
    ina.setupINA226();
    encoder.setupEncoder();

    // Create the semaphore
    SemaphoreBuffer = xSemaphoreCreateBinary();

    // Set the semaphore
    xSemaphoreGive(SemaphoreBuffer);

    // Creating tasks
    xTaskCreatePinnedToCore(dataloggerTask, "Dtatalogger_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(encoderTask, "Encoder_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(inaTask, "INA226_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(
                gpsTask,        // Task function
                "GPS_Task",     // Task name
                10000,          // Stack size
                NULL,           // Parameters
                1,              // Priority
                NULL,           // Task Handle
                1);   // Core number (1 or 0)


    // notifying all tasks have been created 
    Serial.println("All tasks created");
  
}// end setup

// --------------------------------------------------------------------------------------------------- READY TO GO
void dataloggerTask(void *param) {

  while(1) {

    String data = gps.getTimestamp() + ','
                + gps.getLat() + ','
                + gps.getLon() + ','
                + ina.getVoltage() + ','
                + ina.getCurrent() + ','
                + ina.getPower() + ','
                + ina.getConsumption() + ','
                + encoder.getSpeed() + ','
                + encoder.getAverageSpeed() + ','
                + '0';

    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

    datalogger.concatenaArquivo(path, data);

    xSemaphoreGive(SemaphoreBuffer);

    vTaskDelay(1 / portTICK_PERIOD_MS);

  }// end while

}// end task


// --------------------------------------------------------------------------------------------------- READY TO GO
void encoderTask(void *parameter) {

  while (1) {

    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

    encoder.calculaVelocidade(encoder.amostraVoltas());

    xSemaphoreGive(SemaphoreBuffer);

    vTaskDelay(100 / portTICK_PERIOD_MS);

    //encoder.imprimir();

  }// end while

}// end task


// --------------------------------------------------------------------------------------------------- READY TO GO
void inaTask(void *parameter) {

    while (1) {

        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

        ina.atualizaINA226();

        xSemaphoreGive(SemaphoreBuffer);

        vTaskDelay(10 / portTICK_PERIOD_MS);

        //ina.imprimir();

    }// end while

}// end task


// --------------------------------------------------------------------------------------------------- READY TO GO
void gpsTask(void *parameter) {

    while (1) {
    
        gps.liberaGPS();

        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);
        
        gps.atualizaGPS();

        xSemaphoreGive(SemaphoreBuffer);

        //gps.imprimir();

    }//end while

}// end GPS Task


// ---------------------------------------------------------------------------------------------------
void loop() {

}
