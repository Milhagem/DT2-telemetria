#include "encoder.hpp"

volatile byte pulsos;
void contador() {
  pulsos++;
}

void Encoder::setupEncoder() {

  // Variavels para calculo de velocidade e distancia
  // double speed            = 0;
  // double average_speed    = 0;
  // double distancia_total  = 0;

  this->speed = 0;           // km/h
  this->average_speed = 0;   // km/h
  this->distancia_total = 0;

  // Inicalizacao de variaveis de controle
  this->tempo_inicio      = 0;
  this->tempo_speed_old   = 0;
  this->tempo_speed_atual = 0;
  this->tempo_delta       = 0;
  this->tempo_total       = 0;
  this->speed_old         = 0;
  this->distancia_trecho  = 0;
  this->ja_andou          = false;

  attachInterrupt(digitalPinToInterrupt(MEASURE_PIN), contador, FALLING);
}


volatile unsigned long last_pulse_time = 0;
volatile unsigned long tempo_atual = 0;
volatile unsigned long intervalo_tempo = 0;

/*double Encoder::amostraVoltasSamples() {
  double rpm = 0;

  //if (this->speed >= MINIMAL_SPEED) {

  // Calcula o intervalo de tempo desde o último pulso
  unsigned long intervalo_tempo = millis() - last_pulse_time;

  // Atualiza a RPM somente se houver pelo menos 16 pulsos recebido
  if (pulsos > SAMPLES) {

    // Desabilita interrupcao durante o calculo
    detachInterrupt(0);
    rpm = ((60.0 * 1000.0 / SAMPLES) / intervalo_tempo )* pulsos;

    pulsos = 0; // Zera o contador de pulsos após calcular a RPM

    Serial.print("RPM = ");
    Serial.println(rpm, DEC);

    attachInterrupt(0, contador, FALLING);
  }
//}
  return rpm;
}*/


double Encoder::amostraVoltasSamples() {
  double rpm = 0;


  //if (this->speed >= MINIMAL_SPEED) {

    // Calcula o intervalo de tempo desde o último pulso
    

    // Atualiza a RPM somente se houver pelo menos 16 pulsos recebido
    if (pulsos > SAMPLES) {
      tempo_atual = millis();
      intervalo_tempo = tempo_atual - last_pulse_time;
      last_pulse_time = tempo_atual;
      // Desabilita interrupcao durante o calculo
      detachInterrupt(0);
      rpm = ((min_TO_ms / SAMPLES) / intervalo_tempo) * pulsos;

      //Serial.print("RPM = ");
      //Serial.println(rpm);

      pulsos = 0; // Zera o contador de pulsos após calcular a RPM

      attachInterrupt(0, contador, FALLING);
    }
//}
  return rpm;
}


double Encoder::amostraVoltasTimeInterval() {
 double rpm = 0;

 // Atualiza contador a cada segundo
 tempo_atual = millis();
 if (tempo_atual - last_pulse_time >= 100) {
    
    // Desabilita interrupcao durante o calculo
    detachInterrupt(0);
    rpm = ((min_TO_ms / SAMPLES ) / (tempo_atual - last_pulse_time)) * pulsos; 

    last_pulse_time = millis();

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
