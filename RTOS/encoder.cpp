#include "encoder.hpp"

volatile byte pulses;
void counter() {
    pulses++;
}

// Method to set up the encoder
void Encoder::setupEncoder() {
    // Configure the interrupt to detect pulse count
    attachInterrupt(digitalPinToInterrupt(MEASURE_PIN), counter, FALLING);

    // Variables for speed and distance calculations
    double speed = 0;
    double average_speed = 0;
    double total_distance = 0;

    // Initialize control variables
    this->start_time = 0;
    this->last_speed_time = 0;
    this->current_speed_time = 0;
    this->time_delta = 0;
    this->total_time = 0;
    this->previous_speed = 0;
    this->segment_distance = 0;
    this->has_started = false;
}

volatile unsigned long last_pulse_time = 0;

// Method to sample the encoder's revolutions and calculate RPM
double Encoder::sampleRevolutions() {
    int rpm = 0;

    if (this->speed >= MINIMAL_SPEED) {
        // testar Atualiza contador a cada segundo
        //  if (millis() - timeold >= 1000) {
            
        // Desabilita interrupcao durante o calculo
        //    detachInterrupt(0);
        //    rpm = (60 * 1000 / SAMPLES ) / (millis() - timeold) * pulsos; 

        //    timeold = millis();
        //    pulsos = 0;

        //    Serial.print("RPM = ");
        //    Serial.println(rpm, DEC);
            
            // Habilita interrupcao
        //    attachInterrupt(0, contador, FALLING);
        //  }
      


            // Calculate the time interval since the last pulse
            unsigned long time_interval = millis() - last_pulse_time;

            // Update RPM only if at least 16 pulses have been received
            if (pulses > SAMPLES) {
                // Disable the interrupt during the calculation
                detachInterrupt(0);
                rpm = (60 * 1000 / SAMPLES) / time_interval * pulses;

                pulses = 0; // Reset the pulse counter after calculating RPM

                Serial.print("RPM = ");
                Serial.println(rpm, DEC);

                attachInterrupt(0, counter, FALLING);
            }
    }
    return rpm;
}

// Method to calculate the encoder's speed in km/h
void Encoder::calculateSpeed(double rpm) {
    segment_distance = 0;  // meters

    double speed_mpm = rpm * WHEEL_CIRCUMFERENCE; // meters per minute
    this->speed = (speed_mpm * (60.0 / 1000.0)); // km/h

    if (speed != 0 && this->has_started == false) {  // Started moving;
        this->start_time = millis();
        this->last_speed_time = millis();
        this->current_speed_time = millis();
        this->previous_speed = speed_mpm;
        this->has_started = true;
    } else if (this->has_started == true) {
        this->current_speed_time = millis();
        this->total_time = this->current_speed_time - this->start_time;
        this->time_delta = this->current_speed_time - this->last_speed_time;
