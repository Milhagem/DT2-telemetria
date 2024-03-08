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

#include "lte_connection.hpp"
#include "datalogger.hpp"
#include "encoder.hpp"
#include "gps.hpp"
#include "displayTFT.hpp"
//#include "includes/ina226.hpp"


// ---------------------------------------------------------------------------------------------------
//LTE_Connection connection;
Encoder encoder;
//GPS gps;
Datalogger datalogger;
//Ina226 ina;
DisplayTFT display;

// ---------------------------------------------------------------------------------------------------
SemaphoreHandle_t SemaphoreBuffer;
const String path = "/testeEncoder.txt";
String data;

// ---------------------------------------------------------------------------------------------------
void dataloggerTask(void *);
void encoderTask(void *);
//void gpsTask(void *);
//void inaTask(void *);
//void sendDataTask(void *);

// ---------------------------------------------------------------------------------------------------
void setup() {

    SerialMon.begin(115200);

    // Wait a moment to start 
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Setups----------------------------------------------
    //connection.setupLTE();
    //connection.connectGRPS(apn, gprsUser, gprsPass);
    //connection.connectServer(server, resource, port);

    datalogger.setupDatalogger();
    datalogger.abreArquivo(path);

    //gps.setupGPS();
    //gps.liberaGPS();

    //ina.setupINA226();

    encoder.setupEncoder();

    display.setupDisplayTFT();

    //Lcd.setupDisplayLCD();




    // Semáforo-------------------------------------------
    SemaphoreBuffer = xSemaphoreCreateBinary();

    xSemaphoreGive(SemaphoreBuffer);

    // Tasks----------------------------------------------
    xTaskCreatePinnedToCore(dataloggerTask, "Datalogger_Task", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(encoderTask, "Encoder_Task", 10000, NULL, 1, NULL, 1);
    //xTaskCreatePinnedToCore(gpsTask, "GPS_Task", 10000, NULL, 1, NULL, 1);
    //xTaskCreatePinnedToCore(inaTask, "INA226_Task", 10000, NULL, 1, NULL, 1);
    //xTaskCreatePinnedToCore(sendDataTask, "Send_Data_Task", 10000, NULL, 1, NULL, 0);

    // notifying all tasks have been created 
    SerialMon.println("All tasks created");
  
}// end setup

// --------------------------------------------------------------------------------------------------- READY TO GO
void dataloggerTask(void *param) {

  while(1) {

    // data = gps.getTimestamp() + ','
    //      + gps.getLat() + ','
    //      + gps.getLon() + ','
    //      + '0' + ','
    //      + '0' + ','
    //      + '0' + ','
    //      + '0' + ','
    //      + encoder.getSpeed() + ','
    //      + '0' + ','
    //      + '0';

    data = String(0) + ','
         + String(0) + ','
         + String(0) + ','
         + String(0) + ','
         + String(0) + ','
         + String(0) + ','
         + String(0) + ','
         + encoder.getSpeed() + ','
         + String(0) + ','
         + String(0);


    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

    //Serial.println("Mensagem adicionada: ");
    Serial.println(data);
    datalogger.concatenaArquivo(path, data);

    xSemaphoreGive(SemaphoreBuffer);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

  }// end while

}// end task


// --------------------------------------------------------------------------------------------------- READY TO GO
void encoderTask(void *param) {

  while (1) {

    xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

    encoder.calculaVelocidade(encoder.amostraVoltasTimeInterval());

    xSemaphoreGive(SemaphoreBuffer);

    vTaskDelay(100 / portTICK_PERIOD_MS);

    encoder.imprimir();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

  }// end while

}// end task


//--------------------------------------------------------------------------------------------------- READY TO GO
// void gpsTask(void *param) {

//     while (1) {

//         xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);
        
//         gps.atualizaGPS();     

//         xSemaphoreGive(SemaphoreBuffer);

//         vTaskDelay(2 / portTICK_PERIOD_MS);

//         //gps.imprimir();

//     }//end while

// }// end GPS Task


// --------------------------------------------------------------------------------------------------- READY TO GO
// void inaTask(void *param) {

//     while (1) {

//         xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

//         ina.atualizaINA226();

//         xSemaphoreGive(SemaphoreBuffer);

//         vTaskDelay(2 / portTICK_PERIOD_MS);

//         ina.imprimir();

//     }// end while

// }// end task


// --------------------------------------------------------------------------------------------------- TO BE TESTED
// void sendDataTask(void *param) {

//   while (1) {

//       String data = "api_key=" + String("kldv5392") + 
//                     "&rpm=" + String(32) + "&speed=" +  String(44) + "&average_speed=" + String(22) + "&wheel_diameter=" + String(15) +
//                     "&lat=" + String(0) + "&lng=" + String(0) + 
//                     "&celcius=" + String(34) + "&farenheits=" + String(45) + 
//                     "&voltage_battery=" + String(66) + "&current_motor=" + String(curr) + "&power=" + String(53) + "&consumption=" + String(40) + 
//                     "&reading_time=" + String(22/12/2023) + "";

//       xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

//       connection.postRequest(server, resource, data);

//       xSemaphoreGive(SemaphoreBuffer);

//       vTaskDelay(5000 / portTICK_PERIOD_MS);

//   }// end while

// }// end task

// ---------------------------------------------------------------------------------------------------
void loop() {
  display.mostraVelocidadeMedia(encoder.getSpeed().toDouble());
}

