#include "encoder.hpp"

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
  uint8_t n;
  volatile uint8_t nextSample = 0;
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
  double distancia_trecho = 0;
  double distancia_total = 0;

  double rpm = rps * 60 * 0.333;
  double speed_mpm = rps * 60 * 0.333 * wheel_diameter; // metros por minuto (m/min)
  this->speed = (speed_mpm * (60/1000));                // km/h

  if(speed != 0 && this->calc_vel == 0) { // Começou a andar;
    tempo_inicio = millis();
    tempo_speed_old = millis();
    tempo_speed  = millis();
    speed_old = speed_mpm;
    this->ja_andou = true;
  } else if(this->ja_andou == true){  // Ja andou
    tempo_speed = millis();
    tempo_total = tempo_speed - tempo_inicio;
    tempo_delta = tempo_speed_atual - tempo_speed_old;
    distancia_trecho = speed_mpm * (tempo_delta * ms_TO_min);         // metros (m)
    distancia_total = (distancia_trecho + distancia_total)/1000;     // km
    average_speed = distancia_total/ ((tempo_total * ms_TO_min)/60); // km/h
    speed_old = speed_mpm;
    tempo_speed_old = tempo_speed;
  }
}


void Encoder::imprimir() {
  Serial.print("Velocidade: ")
  Serial.print(this->speed);
  Serial.println(" km/ ")
  Serial.print("Velocidade media :");
  Serial.print(this->average_speed);
  Serial.println(" km/h");
  Serial.print("Distancia total :");
  Serial.print(this->distancia_total);
  Serial.println(" km");
  Serial.print("Tempo total :");
  Serial.println(this->tempo_total/1000);
}