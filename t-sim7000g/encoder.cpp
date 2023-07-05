#include "encoder.hpp"

void Encoder::encoderSetup() {
  this->tempo_inicio      = 0;
  this->tempo_speed_old   = 0;
  this->tempo_speed_atual = 0;
  this->tempo_delta       = 0;
  this->tempo_total       = 0;
  this->speed_old         = 0;
  this->distancia_trecho  = 0;
  this->ja_andou          = false;
  
}

double Encoder::amostraVoltas() {
  typedef struct {
    uint16_t ms;
    uint16_t revolutions;
  } sample_t;

  volatile uint8_t nextSample = 0;
  volatile sample_t samples[SAMPLES] = {};

  // Temp vars for computing totals
  volatile sample_t* pSample;
  uint8_t n;
  uint8_t totalRevolutions = 0;
  uint8_t totalSamples = 0;
  uint16_t minMs = 0;
  uint16_t maxMs = 0;
  double rps = 0;
  
  // Iterate thru samples
  for (int i = 0; i < SAMPLES; i++) {
    n = (nextSample + i) % SAMPLES;
    pSample = &samples[n];

    if (pSample->ms > 0) {
      // Good sample
      totalSamples++;
      totalRevolutions += pSample->revolutions;

      minMs = std::min(minMs, (uint16_t)pSample->ms);
      maxMs = std::max(maxMs, (uint16_t)pSample->ms);
    }
  }

  // Compute rps
  if (totalSamples > 0) {
    rps = totalRevolutions * 1.0 / totalSamples;
  }

  // Prepare for a new sample
  if (nextSample >= SAMPLES-1) {
    nextSample = 0;
  } else {
    nextSample++;
  }

  // Clear old sample data
  samples[nextSample].ms = millis();
  samples[nextSample].revolutions = 0;

  return rps;
}


void Encoder::calculaVelocidade(double rps, double wheel_diameter) {
  distancia_trecho = 0; // metros
  distancia_total = 0;  // metros @camposouza CONFERIR SE FAZ SENTIDO distancia_total estar aqui e ser zerada sempre

  double rpm = rps * 60 * 0.333;
  double speed_mpm = rps * 60 * 0.333 * wheel_diameter; // metros por minuto
  this->speed = (speed_mpm * (60/1000));                // km/h

  if(speed != 0 && this->ja_andou == 0) { // Começou a andar;
    this->tempo_inicio = millis();
    this->tempo_speed_old = millis();
    this->tempo_speed_atual = millis();
    this->speed_old = speed_mpm;
    this->ja_andou = true;

  } else if(this->ja_andou == true){
    this->tempo_speed_atual = millis();
    this->tempo_total = this->tempo_speed_atual - this->tempo_inicio;
    this->tempo_delta = this->tempo_speed_atual - this->tempo_speed_old;
    this->distancia_trecho = speed_mpm * (this->tempo_delta * ms_TO_min);
    this->distancia_total = (this->distancia_trecho + this->distancia_total)/1000;
    this->average_speed = this->distancia_total/ ((this->tempo_total * ms_TO_min)/60);
    this->speed_old = speed_mpm;
    this->tempo_speed_old = this->tempo_speed_atual;
  }
}


void Encoder::imprimir() {
  Serial.print("Velocidade: ");
  Serial.print(this->speed);
  Serial.println(" km/ ");
  Serial.print("Velocidade media :");
  Serial.print(this->average_speed);
  Serial.println(" km/h");
  Serial.print("Distancia total :");
  Serial.print(this->distancia_total);
  Serial.println(" km");
  Serial.print("Tempo total :");
  Serial.println(this->tempo_total/1000);
}