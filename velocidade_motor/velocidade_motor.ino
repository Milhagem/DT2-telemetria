#include "velocidade_motor.hpp"

Velocidade_Motor velocidade_motor;

SemaphoreHandle_t SemaphoreBuffer;

void velocidade_motorTask (void *);

void setup() {

  Serial.begin(115200);
 
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  velocidade_motor.motor_setup();

  SemaphoreBuffer = xSemaphoreCreateBinary();

  xSemaphoreGive(SemaphoreBuffer);

  xTaskCreatePinnedToCore( velocidade_motorTask, "Velocidade_Motor_Task", 10000, NULL, 1, NULL, 0);
  
}

void velocidade_motorTask (void *param) {

  while (1) {

      xSemaphoreTake(SemaphoreBuffer, portMAX_DELAY);

      velocidade_motor.calculo_velocidade();

      xSemaphoreGive (SemaphoreBuffer);

      vTaskDelay(100 / portTICK_PERIOD_MS);

      velocidade_motor.imprimir();

      vTaskDelay(2 / portTICK_PERIOD_MS);
  }// end while
}// end Velocidade Motor Task

void loop() {

}