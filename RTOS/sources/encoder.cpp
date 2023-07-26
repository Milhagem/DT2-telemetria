#include "encoder.hpp"

volatile byte pulsos;
void contador() {
  pulsos++;
}

void Encoder::setupEncoder() {
  attachInterrupt(digitalPinToInterrupt(MEASURE_PIN), contador, FALLING);

  double speed            = 0;
  double average_speed    = 0;
  double distancia_total  = 0;

  this->tempo_inicio      = 0;
  this->tempo_speed_old   = 0;
  this->tempo_speed_atual = 0;
  this->tempo_delta       = 0;
  this->tempo_total       = 0;
  this->speed_old         = 0;
  this->distancia_trecho  = 0;
  this->ja_andou          = false;
}

unsigned long timeold = 0;
double Encoder::amostraVoltas() {
  int rpm = 0;

  // Atualiza contador a cada segundo
  if (millis() - timeold >= 1000) {
    
    // Desabilita interrupcao durante o calculo
    detachInterrupt(0);
    rpm = (60 * 1000 / SAMPLES ) / (millis() - timeold) * pulsos; 

    timeold = millis();
    pulsos = 0;

    Serial.print("RPM = ");
    Serial.println(rpm, DEC);
    
    // Habilita interrupcao
    attachInterrupt(0, contador, FALLING);
  }
  return rpm;
}


void Encoder::calculaVelocidade(double rpm) {
  distancia_trecho = 0;  // metros
  
  double speed_mpm = rpm * WHEEL_CIRCUMFERANCE; // metros por minuto
  this->speed = (speed_mpm * (60.0/1000.0));        // km/h

  if(speed != 0 && this->ja_andou == false) {  // Começou a andar;
    this->tempo_inicio = millis();
    this->tempo_speed_old = millis();
    this->tempo_speed_atual = millis();
    this->speed_old = speed_mpm;
    this->ja_andou = true;

  } else if (this->ja_andou == true){
    this->tempo_speed_atual = millis();
    this->tempo_total = this->tempo_speed_atual - this->tempo_inicio;
    this->tempo_delta = this->tempo_speed_atual - this->tempo_speed_old;
    this->distancia_trecho = speed_mpm * (this->tempo_delta * ms_TO_min);
    this->distancia_total = this->distancia_total + this->distancia_trecho/1000.0;
    this->average_speed = this->distancia_total / ((this->tempo_total * ms_TO_min)/60.0);
    this->speed_old = speed_mpm;
    this->tempo_speed_old = this->tempo_speed_atual;
  }
}


void Encoder::imprimir() {
  Serial.print("Velocidade: ");
  Serial.print(this->speed);
  Serial.println(" km/h ");
  Serial.print("Velocidade media :");
  Serial.print(this->average_speed);
  Serial.println(" km/h");
  Serial.print("Distancia total :");
  Serial.print(this->distancia_total);
  Serial.println(" km");
  Serial.print("Tempo total :");
  Serial.println(this->tempo_total/1000.0);
}
