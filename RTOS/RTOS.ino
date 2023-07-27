/**
Versão 2.0 do código de telemetria do DT2
--------------------
Autores:
    

--------------------
Funcionalidades:
    GPS funcional
    Datalogger funcional
*/

#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "datalogger.hpp"
#include "encoder.hpp"
#include "gps.hpp"
#include "lm35.hpp"
#include "ina226.hpp"
//#include "displayTFT.hpp"

Encoder encoder;

GPS gps;

Datalogger datalogger;
const String path = "/teste_RTOS.txt";

LM35 lm35;

Ina226 ina;

//DisplayTFT display; /*display TFT desabilitado temporariamente*/


// ---------------------------------------------------------------------------------------------------
SemaphoreHandle_t SemaphoreBuffer;

// ---------------------------------------------------------------------------------------------------
void setup() {

    Serial.begin(115200);

    // Wait a moment to start (so we don't miss Serial output)
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Setups
    gps.setupGPS();
    datalogger.dataloggerSetup();
    datalogger.abreArquivo(path);
    ina.setupINA226();
    encoder.setupEncoder();

    // Create the semaphore
    SemaphoreBuffer = xSemaphoreCreateBinary();

    // Set the semaphore
    xSemaphoreGive(SemaphoreBuffer);

    // Creating tasks
    xTaskCreatePinnedToCore(encoderTask, "Encoder_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(inaTask, "INA226_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(lm35Task, "LM35_Task", 10000, NULL, 1, NULL, 1);
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
void encoderTask(void *parameter) {

  while (1) {

    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

    encoder.calculaVelocidade(encoder.amostraVoltas());

    xSemaphoreGive(SemaphoreBuffer);

    encoder.imprimir();
  }

}

// --------------------------------------------------------------------------------------------------- READY TO GO
void inaTask(void *parameter) {

    while (1) {

        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

        ina.atualizaINA226();

        xSemaphoreGive(SemaphoreBuffer);

        ina.imprimir();

    }// end while

}// end task


// --------------------------------------------------------------------------------------------------- TO BE TESTED
void lm35Task(void *parameter) {

    while (1) {

        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

        lm35.atualizaLM35();

        xSemaphoreGive(SemaphoreBuffer);

        lm35.imprimir();


    }// end while

}// end lm35 task


// --------------------------------------------------------------------------------------------------- READY TO GO
void gpsTask(void *parameter) {

    while (1) {
    
        // verify and enable the GPS modem
        gps.liberaGPS();

        // Take the semaphore to access the shared resource
        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);
        
        // Update de data from the GPS
        gps.atualizaGPS();

        // Release the semaphore
        xSemaphoreGive(SemaphoreBuffer);

        // Serial print the GPS data
        gps.imprimir();

    }//end while

}// end GPS Task


// ---------------------------------------------------------------------------------------------------
void loop() {

  // Armazenamento em string no cartão SD
  datalogger.concatenaArquivo(path,
    gps.getTimestamp(),
    String(ina.getVoltage()),
    String(ina.getCurrent()),
    String(ina.getPower()),
    String(ina.getConsumption()),
    String(lm35.getTemperatura()),
    String(encoder.getSpeed()),
    String(encoder.getAverageSpeed()),
    String(gps.getLat()),
    String(gps.getLon())
    ); 

  vTaskDelay(1 / portTICK_PERIOD_MS);

}
