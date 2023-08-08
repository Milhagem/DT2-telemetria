/**
 * @file RTOS.ino
 * @author Felipe Facury
 * @author Thiago (Putão)
 * @brief Código em sistema operacional em tempo real para a telemetria do DT2
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "includes/lte_connection.hpp"
#include "includes/datalogger.hpp"
#include "includes/encoder.hpp"
#include "includes/gps.hpp"
#include "includes/ina226.hpp"


// ---------------------------------------------------------------------------------------------------
LTE_Connection connection;
Encoder encoder;
GPS gps;
Datalogger datalogger;
Ina226 ina;

// ---------------------------------------------------------------------------------------------------
SemaphoreHandle_t SemaphoreBuffer;
const String path = "/teste_RTOS.txt";
String data;

// ---------------------------------------------------------------------------------------------------
void gpsTask(void *);
void encoderTask(void *);
void dataloggerTask(void *);
void inaTask(void *);
void sendDataTask(void *);

// ---------------------------------------------------------------------------------------------------
void setup() {

    SerialMon.begin(115200);

    // Wait a moment to start 
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Setups
    connection.setupLTE();
    connection.connectGRPS();
    connection.connectServer();

    datalogger.setupDatalogger();
    datalogger.abreArquivo(path);
    gps.setupGPS();
    gps.liberaGPS();
    ina.setupINA226();
    encoder.setupEncoder();


    // Create the semaphore
    SemaphoreBuffer = xSemaphoreCreateBinary();

    // Set the semaphore
    xSemaphoreGive(SemaphoreBuffer);

    // Creating tasks
    xTaskCreatePinnedToCore(sendDataTask, "Send_Data_Task", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(dataloggerTask, "Datalogger_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(encoderTask, "Encoder_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(inaTask, "INA226_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(
                gpsTask,        // Task function
                "GPS_Task",     // Task name
                10000,          // Stack size
                NULL,           // Parameters
                1,              // Priority
                NULL,           // Task Handle
                1);             // Core number (1 or 0)


    // notifying all tasks have been created 
    SerialMon.println("All tasks created");
  
}// end setup

// --------------------------------------------------------------------------------------------------- TO BE TESTED
void sendDataTask(void *param) {

  while (1) {

    data = gps.getTimestamp() + ','
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

      connection.postRequest(data);

      xSemaphoreGive(SemaphoreBuffer);

      vTaskDelay(2 / portTICK_PERIOD_MS);

  }// end while

}// end task


// --------------------------------------------------------------------------------------------------- READY TO GO
void dataloggerTask(void *param) {

  while(1) {

    data = gps.getTimestamp() + ','
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

    Serial.println("concatenando");
    datalogger.concatenaArquivo(path, data);

    xSemaphoreGive(SemaphoreBuffer);

    vTaskDelay(2 / portTICK_PERIOD_MS);

  }// end while

}// end task


// --------------------------------------------------------------------------------------------------- READY TO GO
void encoderTask(void *param) {

  while (1) {

    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

    encoder.calculaVelocidade(encoder.amostraVoltasSamples());

    xSemaphoreGive(SemaphoreBuffer);

    vTaskDelay(100 / portTICK_PERIOD_MS);

    encoder.imprimir();

    vTaskDelay(2 / portTICK_PERIOD_MS);

  }// end while

}// end task


// --------------------------------------------------------------------------------------------------- READY TO GO
void inaTask(void *param) {

    while (1) {

        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

        ina.atualizaINA226();

        xSemaphoreGive(SemaphoreBuffer);

        vTaskDelay(2 / portTICK_PERIOD_MS);

        ina.imprimir();

    }// end while

}// end task


// --------------------------------------------------------------------------------------------------- READY TO GO
void gpsTask(void *param) {

    while (1) {

        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);
        
        gps.atualizaGPS();

        xSemaphoreGive(SemaphoreBuffer);

        vTaskDelay(2 / portTICK_PERIOD_MS);

        gps.imprimir();

    }//end while

}// end GPS Task



// ---------------------------------------------------------------------------------------------------
void loop() {

}

