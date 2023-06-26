#include "encoder.hpp"

void encoder::amostraVoltas() {
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
}


void encoder::calculaVelocidade(double rps, double wheel_diameter) {
  double distancia_trecho = 0;
  double distancia_total = 0;

  double rpm = rps * 60 * 0.333;
  double speed_mpm = rps * 60 * 0.333 * wheel_diameter; // metros por minuto (m/min)
  this-> speed = (speed_mpm * (60/1000));                 // km/h

  if(speed != 0 && speed_velho == 0){
    Serial.println("Começou a andar");
    tempo_Inicio = millis();
    tempo_speedVelho = millis();
    tempo_speedAtual= millis();
    speed_velho = speed_mpm;
    andou = 1;
  }
  else if(andou == 1){
    tempo_speedAtual = millis();
    tempo_total = tempo_speedAtual - tempo_Inicio;
    tempo_dif = tempo_speedAtual - tempo_speedVelho;
    distancia_trecho = speed_mpm * (tempo_dif * ms_to_min); // metros
    distancia_total = (distancia_trecho + distancia_total)/1000; // km
    average_speed = distancia_total/ ((tempo_total * ms_to_min)/60); // km/h
    speed_velho = speed_mpm;
    tempo_speedVelho = tempo_speedAtual;
  }
}


void encoder::imprimir() {
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