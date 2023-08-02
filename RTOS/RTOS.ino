/**
Versão mais recente do código de telemetria do DT2
--------------------
Autores:
    [Insira aqui o nome dos autores]

--------------------
Funcionalidades:
    - GPS
    - Datalogger
    - Encoder
    - INA226

Observação:
  Quando realizar testes com esse código,
  lembre de descomentar os métodos de impressão
  de dados no monitor serial
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
void gpsTask(void *);
void encoderTask(void *);
void dataloggerTask(void *);
void inaTask(void *);

// ---------------------------------------------------------------------------------------------------
void setup() {
    Serial.begin(115200);

    // Wait a moment to start
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Setups
    datalogger.setupDatalogger();
    datalogger.abreArquivo(path);
    gps.setupGPS();
    ina.setupINA226();
    encoder.setupEncoder();

    // Create the semaphore
    SemaphoreBuffer = xSemaphoreCreateBinary();

    // Set the semaphore
    xSemaphoreGive(SemaphoreBuffer);

    // Creating tasks
    xTaskCreatePinnedToCore(dataloggerTask, "Datalogger_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(encoderTask, "Encoder_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(inaTask, "INA226_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(gpsTask, "GPS_Task", 10000, NULL, 1, NULL, 1);

    // notifying all tasks have been created
    Serial.println("All tasks created");
}

// --------------------------------------------------------------------------------------------------- READY TO GO
void dataloggerTask(void *param) {
    while (1) {
        String data = gps.getTimestamp() + ',' +
                      gps.getLat() + ',' +
                      gps.getLon() + ',' +
                      ina.getVoltage() + ',' +
                      ina.getCurrent() + ',' +
                      ina.getPower() + ',' +
                      ina.getConsumption() + ',' +
                      encoder.getSpeed() + ',' +
                      encoder.getAverageSpeed() + ',' +
                      '0';

        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

        Serial.println("concatenando");
        datalogger.concatenaArquivo(path, data);

        xSemaphoreGive(SemaphoreBuffer);

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

// --------------------------------------------------------------------------------------------------- READY TO GO
void encoderTask(void *param) {
    while (1) {
        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

        encoder.calculaVelocidade(encoder.amostraVoltas());

        xSemaphoreGive(SemaphoreBuffer);

        vTaskDelay(100 / portTICK_PERIOD_MS);

        // encoder.imprimir();
    }
}

// --------------------------------------------------------------------------------------------------- READY TO GO
void inaTask(void *param) {
    while (1) {
        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

        ina.atualizaINA226();

        xSemaphoreGive(SemaphoreBuffer);

        vTaskDelay(10 / portTICK_PERIOD_MS);

        // ina.imprimir();
    }
}

// --------------------------------------------------------------------------------------------------- READY TO GO
void gpsTask(void *param) {
    while (1) {
        gps.liberaGPS();

        xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

        gps.atualizaGPS();

        xSemaphoreGive(SemaphoreBuffer);

        // gps.imprimir();
    }
}

// ---------------------------------------------------------------------------------------------------
void loop() {
    
}
