#include "encoder.hpp"

float velocidade::resolucao {
  // compute total revolutions and revolutions per second
  totalRevolutions = 0;
  totalSamples = 0;
  minMs = 0;
  maxMs = 0;
  rps = 0;

  // iterate thru samples
  for (int i=0;i<SAMPLES;i++) {
    n = (nextSample+i) % SAMPLES;
    pSample = &samples[n];

    if (pSample->ms > 0) {
      // good sample
      totalSamples++;
      totalRevolutions += pSample->revolutions;

      minMs = std::min(minMs, (uint16_t)pSample->ms);
      maxMs = std::max(maxMs, (uint16_t)pSample->ms);
    }
  }

  // compute rps
  if (totalSamples > 0) {
    rps = totalRevolutions * 1.0 / totalSamples;
  }

  // prepare for a new sample
  if (nextSample >= SAMPLES-1) {
    nextSample = 0;
  } else {
    nextSample++;
  }

  // clear old sample data
  samples[nextSample].ms = millis();
  samples[nextSample].revolutions = 0;
  rpm = rps * 60 * 0.333;
  speed_mpm = rps * 60 * 0.333 * WHEEL_CIRCUMFERANCE; // em metros por minuto
  speed = (speed_mpm*(60/1000)); // Velocidade instantanea em km/h
  Serial.print("speed_atual :");
  Serial.println(speed);
  if(speed!=0 && speed_velho==0){
    Serial.println("Começou a andar");
    tempo_Inicio = millis();
    tempo_speedVelho = millis();
    tempo_speedAtual= millis();
    speed_velho = speed_mpm;
    andou = 1;
  } else if(andou == 1){
    tempo_speedAtual = millis();
    tempo_total = tempo_speedAtual - tempo_Inicio;
    tempo_dif = tempo_speedAtual - tempo_speedVelho;
    distancia_trecho = speed_mpm * (tempo_dif * ms_to_min); // em metros
    distancia_total = (distancia_trecho + distancia_total)/1000; // em km
    average_speed = distancia_total/ ((tempo_total * ms_to_min)/60); //em km/h
    speed_velho = speed_mpm;
    tempo_speedVelho = tempo_speedAtual;
  }
}

void velocidade::param_velo{
  Serial.print("velocidade media :");
  Serial.println(average_speed);
  Serial.print("distancia_total :");
  Serial.println(distancia_total);
  Serial.print("tempo_total :");
  Serial.println(tempo_total/1000);
  wheel_diameter = WHEEL_CIRCUMFERANCE;
}